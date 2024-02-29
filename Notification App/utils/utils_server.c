#include "common.h"
#include "helpers.h"

// citeste de la stdin
int8_t get_stdin_input () {
    char buf[MSG_MAXSIZE + 1];
    memset(buf, 0, MSG_MAXSIZE + 1);
    fgets(buf, sizeof(buf), stdin);
    if (strcmp(buf, "exit\n") == 0)
        return 0;
    else
        perror("unkown command");

    return 1;
}

// accepta o cerere de conexiune TCP si citeste ID ul subscriberului
struct subscriber* accept_sub_connection (int listenfd, int num_subs, struct subscriber *subs) {
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    int newsockfd = accept(listenfd, (struct sockaddr *)&cli_addr, &cli_len);
    DIE(newsockfd < 0, "accept");

    uint16_t ID_len;
    int rc = recv_all(newsockfd, &ID_len, sizeof(uint16_t));
    DIE(rc < 0, "ID recv");

    struct subscriber *new_sub = calloc(sizeof(struct subscriber), 1);
    new_sub->fd = newsockfd;
    new_sub->ID = calloc(ID_len + 1, 1);

    rc = recv_all(newsockfd, (*new_sub).ID, ID_len);
    DIE(rc < 0, "ID recv");

    if (sub_exists(new_sub->ID, num_subs, subs) == 1) {
        printf("Client %s already connected.\n", new_sub->ID);
        new_sub->ID = NULL;

        return new_sub;
    }

    printf("New client %s connected from %s:%d.\n",
                 (*new_sub).ID, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

    return new_sub;
}

// verifica daca subscriberul este deja logat
int sub_exists (char* ID, int num_subs, struct subscriber *subs) {
    for (int i = 0; i < num_subs; i++) {
        if (strcmp(ID, subs[i].ID) == 0 && subs[i].fd != -1) {
            return 1;
        }
    }
    return 0;
}

// daca un subscriber se reconecteaza, ii trimite pachetele restante
// daca e prima oara cand se conecteaza il adauga in lista de subscriberi
int update_subs_add (int num_subs, struct subscriber **subs, struct subscriber *new_sub,
                    int *num_messages, struct message **messages) {
    for (int i = 0; i < num_subs; i++) {
        if (strcmp((*subs)[i].ID, new_sub->ID) == 0) {
            // se reconecteaza, deci o sa trebuiasca sa trimit mesajele restante daca exista

            (*subs)[i].fd = new_sub->fd;
            send_restant_messages(num_messages, messages, (*subs) + i, num_subs, subs);

            free(new_sub->ID);
            free(new_sub);

            return num_subs;
        }
    }

    num_subs++;
    *subs = realloc((*subs), (num_subs + 1) * sizeof(struct subscriber));
    memset((*subs) + num_subs - 1, 0, sizeof(struct subscriber));
    DIE(!(*subs), "subs realloc");
    (*subs)[num_subs - 1] = *new_sub;

    // un subscriber nou nu are mesaje restante
    (*subs)[num_subs - 1].msg_num = 0;
    (*subs)[num_subs - 1].msg_index = NULL;

    return num_subs;
}

// functie ajutatoare, printeaza subscriberii si numarul de mesaje restante
void print_subs (int num_subs, struct subscriber *subs) {
    for (int i = 0; i < num_subs; i++) {
        printf("Subscriberul %s are %d mesaje restante\n", subs[i].ID, subs[i].msg_num);
    }
    printf("\n");
}

// inchide socketul clientului care s-a deconectat si il scoate din lista pentru poll
// si marcheaza cu -1 fd ul in lista de subscriberi
void close_sub_connection (int num_subs, struct subscriber **subs,
                        int *num_clients, struct pollfd **poll_fds, int closefd) {
    
    int i;
    for (i = 0; i < (*num_clients); i++) {
        if ((*poll_fds)[i].fd == closefd) {
            close((*poll_fds)[i].fd);
            break;
        }
    }
    (*poll_fds)[i] = (*poll_fds)[*num_clients - 1];
    (*num_clients) -= 1;
    (*poll_fds) = realloc((*poll_fds), *num_clients * sizeof(struct pollfd));
    DIE(!(*poll_fds), "realloc");

    for (i = 0; i < num_subs; i++) {
        if ((*subs)[i].fd == closefd) {
            (*subs)[i].fd = -1; // subscriber is offline
            printf("Client %s disconnected.\n", (*subs)[i].ID);
        }
    }
}

// adauga un nou file descriptor in lista pentru poll
int update_poll_fds (int num_clients, struct pollfd **poll_fds, int newfd) {
    num_clients++;
    *poll_fds = realloc((*poll_fds), num_clients * sizeof(struct pollfd));
    memset((*poll_fds) + num_clients - 1, 0, sizeof(struct pollfd));
    DIE(!(*poll_fds), "poll realloc");
    (*poll_fds)[num_clients - 1].fd = newfd;
    (*poll_fds)[num_clients - 1].events = POLLIN;
    (*poll_fds)[num_clients - 1].revents = 0;

    return num_clients;
}

// intoarce un socket care asculta cereri de conexiune TCP
// folosind portul specificat
int get_TCP_socket (uint16_t port) {
    // Obtinem un socket TCP pentru receptionarea conexiunilor
    int TCP_socket = socket(AF_INET, SOCK_STREAM, 0);
    DIE(TCP_socket < 0, "TCP Socket");

    // Completăm in serv_addr adresa serverului, familia de adrese si portul
    // pentru conectare
    struct sockaddr_in serv_addr;
    socklen_t socket_len = sizeof(struct sockaddr_in);

    // Facem adresa socket-ului reutilizabila, ca sa nu primim eroare in caz ca
    // rulam de 2 ori rapid
    int enable = 1;
    if (setsockopt(TCP_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");
    
    int nagle = 1;
    if (setsockopt(TCP_socket, IPPROTO_TCP, TCP_NODELAY, &nagle, sizeof(nagle)) < 0)
        perror("setsockopt(TCP_NODELAY) failed");

    memset(&serv_addr, 0, socket_len);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Asociem adresa serverului cu socketul creat folosind bind
    int rc = bind(TCP_socket, (const struct sockaddr *)&serv_addr, sizeof(serv_addr));
    DIE(rc < 0, "TCP bind");

    return TCP_socket;
}

// intoarce un socket pe care se primesc date de la clienti UDP
// folosind portul specificat
int get_UDP_socket (uint16_t port) {
    int UDP_socket = socket(AF_INET, SOCK_DGRAM, 0);
    DIE(UDP_socket < 0, "UDP Socket");

    struct sockaddr_in servaddr;
    
    // Facem adresa socket-ului reutilizabila, ca sa nu primim eroare in caz ca
    // rulam de 2 ori rapid
    int enable = 1;
    if (setsockopt(UDP_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");


    memset(&servaddr, 0, sizeof(servaddr)); 
    
    // Filling server information 
    servaddr.sin_family = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY = 0.0.0.0
    servaddr.sin_port = htons(port); 
    
    // Bind the socket with the server address 
    int rc = bind(UDP_socket, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    DIE(rc < 0, "UDP bind");

    return UDP_socket;
}

// citeste un pachet primit de la un client UDP
struct message_packet* read_UDP_message (int UDP_fd) {
    struct sockaddr_in client_address;
    socklen_t len = sizeof(client_address);

    memset(&client_address, 0, sizeof(client_address));
    int max_len = 1552;
    char *buf = calloc(max_len, 1);

    DIE(!buf, "calloc UDP");

    int received = recvfrom(UDP_fd, buf, max_len, MSG_WAITALL, (struct sockaddr *)&client_address, &len);
    buf[received] = '\0';

    struct message_packet *UDP_message = calloc(sizeof(struct message_packet), 1);
    UDP_message->UDP_addr = client_address.sin_addr;
    UDP_message->UDP_port = client_address.sin_port;
    UDP_message->message_len = received + 1;
    UDP_message->message = calloc(UDP_message->message_len, 1);
    memcpy(UDP_message->message, buf, UDP_message->message_len);

    return UDP_message;
}

// initializeaza o structura de mesaje UDP
struct message* initialize_message (int UDP_fd) {
    struct message *msg = calloc(sizeof(struct message), 1);
    DIE(!msg, "calloc");
    msg->packet = read_UDP_message(UDP_fd);
    msg->SF_remaining = 0;

    return msg;
}

// returneaza topicul unui mesaj
char* get_topic (struct message* msg) {
    char *topic = calloc(51, 1);
    DIE(!topic, "topic calloc");

    memcpy(topic, msg->packet->message, 50);
    return topic;
}

// primeste o cerere de subscribe/unsubscribe de la un client
struct subscribe_packet* receive_subscriber_packet (int sockfd) {
    struct subscribe_packet* sub_packet = calloc(sizeof(struct subscribe_packet), 1);
    memset(sub_packet, 0, sizeof(struct subscribe_packet));
    DIE(!sub_packet, "calloc");

    int rc = recv_all(sockfd, &(sub_packet->subscribe), sizeof(int8_t));
    if (rc == 0) { // clientul a inchis conexiunea
        return NULL;
    }

    rc = recv_all(sockfd, &(sub_packet->topic_len), sizeof(uint16_t));
    DIE(!rc, "recv_all");

    sub_packet->topic = calloc(sub_packet->topic_len + 1, 1);
    rc = recv_all(sockfd, sub_packet->topic, sub_packet->topic_len);
    DIE(!rc, "recv_all");
    sub_packet->topic[sub_packet->topic_len] = '\0';

    rc = recv_all(sockfd, &(sub_packet->SF), sizeof(int8_t));
    DIE(!rc, "recv_all");

    return sub_packet;
}

// returneaza subscriberul care foloseste un anumit fd
struct subscriber* get_subscriber_address (int fd, int num_subs, struct subscriber *subs) {
    for (int i = 0; i < num_subs; i++) {
        if (subs[i].fd == fd)
            return &subs[i];
    }

    return NULL;
}

// cauta in lista de topicuri un topic cu un anumit nume si il returneaza
// sau null daca nu gaseste
struct topic* get_topic_address (int num_topics, struct topic *topics, char* name) {
    for (int i = 0; i < num_topics; i++) {
        if (strcmp(topics[i].name, name) == 0) {
            return &topics[i];
        }
    }

    return NULL;
}

// returneaza indexul unui subscriber din lista de subscriberi
int get_sub_index (char* ID, int num_subs, struct subscriber *subs) {
    for (int i = 0; i < num_subs; i++) {
        if (strcmp(ID, subs[i].ID) == 0) {
            return i;
        }
    }

    return -1;
}

// manageuieste o cerere de abonare pe un topic
int8_t manage_subscribing (int *num_topics, struct topic **topics, int sub_index,
                            char* topic_name, int8_t SF) {
    /*
    verific daca exista topicul
      - daca nu exista, creez un nou topic si adaug clientul in lista de subs
      - daca exista, verific daca clientul se afla deja in lista de subs
        * daca exista, ii updatez datele (SF)
        * daca nu exista, il adaug
    */

    struct topic *current_topic = get_topic_address (*num_topics, *topics, topic_name);
    if (!current_topic) { // topicul nu exista
        *num_topics += 1;
        (*topics) = realloc((*topics), *num_topics * sizeof(struct topic));
        DIE(!(*topics), "realloc");

        (*topics)[*num_topics - 1].name = calloc(strlen(topic_name) + 1, 1);
        (*topics)[*num_topics - 1].subs_index = calloc(sizeof(int), 1);
        (*topics)[*num_topics - 1].SF = calloc(sizeof(int8_t), 1);
        DIE(!(*topics)[*num_topics - 1].name ||
                !(*topics)[*num_topics - 1].subs_index || !(*topics)[*num_topics - 1].SF, "calloc");

        // adaug numele topicului
        memcpy((*topics)[*num_topics - 1].name, topic_name, strlen(topic_name));
        
        // adaug numarul de subs
        (*topics)[*num_topics - 1].topic_subs_number = 1;
        
        // adaug adresa subscriberului curent
        (*topics)[*num_topics - 1].subs_index[0] = sub_index;
        
        // adaug optiunea de SF a subscriberului curent
        (*topics)[*num_topics - 1].SF[0] = SF;
        
        return 1;
    }

    int client_already_subscribed = 0;

    for (int i = 0; i < current_topic->topic_subs_number; i++) {
        if (sub_index == current_topic->subs_index[i]) { // clientul deja se afla in lista de subs
            current_topic->SF[i] = SF;
            client_already_subscribed = 1;
        }
    }

    if (!client_already_subscribed) {
        current_topic->topic_subs_number++;
        current_topic->subs_index = realloc (current_topic->subs_index,
                        current_topic->topic_subs_number * sizeof(int));
        current_topic->SF = realloc (current_topic->SF,
                        current_topic->topic_subs_number * sizeof(int));
        DIE(!current_topic->subs_index || !current_topic->SF, "realloc");

        current_topic->subs_index[current_topic->topic_subs_number - 1] = sub_index;
        current_topic->SF[current_topic->topic_subs_number - 1] = SF;
    }

    return 1;
}

// managuieste o cerere de dezabonare pe un topic
int8_t manage_unsubscribing (int num_topics, struct topic *topics, int sub_index, char* topic_name,
                             int num_subs, struct subscriber *subs) {
    /*
    -1 => topicul nu exista
    -2 => clientul nu este abonat la topic

    verificam daca exista topicul
      - daca nu exista, intorc -1 si trimit eroare inapoi la client
      - daca exista, verific daca clientul se afla in lista de subscriberi
        * daca nu exista, intorc -2 si trimit eroare inapoi la client
        * daca exista, scot clientul si SF ul si realloc() vectorii
    */

    struct topic *current_topic = get_topic_address (num_topics, topics, topic_name);
    if (!current_topic) {   // topicul nu exista
        return -1;
    }
    
    int user_exists = 0;
    int index = 0;

    // verific daca userul exista in lista de subs
    for (index = 0; index < current_topic->topic_subs_number; index++) {
        if (current_topic->subs_index[index] == sub_index) {
            user_exists = 1;
            break;
        }
    }

    if (user_exists) { // userul se afla la adresa index in vectorul subs_index
        current_topic->subs_index[index] = current_topic->subs_index[current_topic->topic_subs_number - 1];
        current_topic->SF[index] = current_topic->SF[current_topic->topic_subs_number - 1];
        current_topic->topic_subs_number--;

        if (current_topic->topic_subs_number > 0) {
            current_topic->subs_index = realloc (current_topic->subs_index, current_topic->topic_subs_number * sizeof(int));
            current_topic->SF = realloc (current_topic->SF, current_topic->topic_subs_number * sizeof(int));

            DIE(!current_topic->subs_index || !current_topic->SF, "realloc");
        } else {
            current_topic->subs_index = NULL;
            current_topic->SF = NULL;
        }
        // subscriberul a fost eliminat cu succes
        return 1;
    }

    return -2;
}

// managuieste o cerere primita de la un client TCP (subscribe/unsubscribe)
void manage_subscriber_packet (int sockfd, struct subscribe_packet *sub_packet, int *num_topics, struct topic **topics,
                                int num_subs, struct subscriber *subs) {
    
    // gaseste subscriberul care a trimis pachetul
    struct subscriber* current_sub = get_subscriber_address(sockfd, num_subs, subs);
    int sub_index = get_sub_index (current_sub->ID, num_subs, subs);

    DIE(!current_sub, "no sub found");

    if (sub_packet->subscribe == 1) { // clientul se aboneaza
        int8_t confirmation = manage_subscribing(num_topics, topics, sub_index, sub_packet->topic, sub_packet->SF);

        int send = send_all(sockfd, &confirmation, sizeof(int8_t));
        DIE(send < 0, "send_all");
    } else if (sub_packet->subscribe == 0) { // clientul se dezaboneaza
        int8_t confirmation = manage_unsubscribing(*num_topics, *topics, sub_index, sub_packet->topic,
                                                   num_subs, subs);

        int send = send_all(sockfd, &confirmation, sizeof(int8_t));
        DIE(send < 0, "send_all");
    }
}

// printeaza continutul listei de topicuri
void print_topics (int num_topics, struct topic *topics) {
    for (int i = 0; i < num_topics; i++) {
        printf("name: %s - subs_num: %hi\n", topics[i].name, topics[i].topic_subs_number);
        for (int j = 0; j < topics[i].topic_subs_number; j++) {
            printf("| %d - %d |", topics[i].subs_index[j], topics[i].SF[j]);
        }
        printf("\n\n");
    }
}

// trimite un pachet UDP catre un subscriber
void send_message_packet (int fd, struct message_packet msg) {
    int sent;

    int8_t packet_confirmation = 1; // 1 inseamna ca trimis un pached de la un client UDP
                                    // 0 inseamna ca am inchis serverul
    sent = send_all(fd, &packet_confirmation, sizeof(int8_t));

    // trimite adresa IP
    sent = send_all(fd, &(msg.UDP_addr), sizeof (struct in_addr));
    DIE(sent < 0, "send_all");

    // trimite portul
    sent = send_all(fd, &(msg.UDP_port), sizeof (unsigned short));
    DIE(sent < 0, "send_all");

    // trimite lungimea mesajului
    sent = send_all(fd, &(msg.message_len), sizeof (uint16_t));
    DIE(sent < 0, "send_all");

    // trimite mesajul
    sent = send_all(fd, msg.message, msg.message_len);
    DIE(sent < 0, "send_all");
}

// se ocupa de pachete primite pe socketul UDP care trebuie trimise catre subscriberi
// sau puse in lista de pachete pana cand sunt trimise si subscriberilor offline cu sf 1
short manage_message_packet (int *num_topics, struct topic **topics, struct message msg,
                           int num_subs, struct subscriber *subs, int num_messages) {
    /*
    verific daca topicul mesajului exista
      * daca nu exista, il creez si nu trebuie trimis mesajul nimanui
      * daca exista, iterez prin lista de subs si il trimit tuturor celor online,
        si incrementez sf_num pentru fiecare sub offline cu sf 1
    */

   struct topic *msg_topic = get_topic_address(*num_topics, *topics, msg.packet->message);

    if (!msg_topic) { // topicul nu exista
        *num_topics += 1;
        (*topics) = realloc((*topics), *num_topics * sizeof(struct topic));
        DIE(!(*topics), "realloc");

        (*topics)[*num_topics - 1].name = calloc(strlen(msg.packet->message) + 1, 1);
        (*topics)[*num_topics - 1].subs_index = NULL; // nu exista subs
        (*topics)[*num_topics - 1].SF = NULL;         // nu exista subs
        DIE(!(*topics)[*num_topics - 1].name, "calloc");

        // adaug numele topicului
        memcpy((*topics)[*num_topics - 1].name, msg.packet->message, strlen(msg.packet->message));
        
        // adaug numarul de subs
        (*topics)[*num_topics - 1].topic_subs_number = 0;

        return 0;
    }

    // topicul exista

    int sf_num = 0;
    for (int i = 0; i < msg_topic->topic_subs_number; i++) {
        struct subscriber *current_sub = subs + msg_topic->subs_index[i];
        if (current_sub->fd == -1) { // sub offline
            if (msg_topic->SF[i] == 1) {

                // adaug indexul mesajului curent la finalul listei de mesaje a subscriberului
                (current_sub->msg_num)++;
                current_sub->msg_index = realloc(current_sub->msg_index, current_sub->msg_num * sizeof(int));
                DIE(!current_sub->msg_index, "realloc");
                current_sub->msg_index[current_sub->msg_num - 1] = num_messages;

                // numarul de clienti carora trebuie sa le fie trimis mesajul curent
                sf_num++;
            }
            continue; // e offline si are sf 0
        }

        send_message_packet(current_sub->fd, *msg.packet);
    }

    return sf_num;
}

// serverul a primit comanda exit, deci trimite fiecarui subscriber confirmarea de inchidere,
// inchide socketii si elibereaza memoria din vectorii folositi
void close_server (struct pollfd **poll_fds, int num_subs, struct subscriber **subs,
                   int num_topics, struct topic **topics, int num_messages, struct message **messages) {

    // inchide fiecare subscriber
    int8_t confirmation = 0;
    for (int i = 0; i < num_subs; i++) {
        if ((*subs)[i].fd == -1) {
            continue;
        }
        int sent = send_all((*subs)[i].fd, &confirmation, sizeof(int8_t));
        close((*subs)[i].fd);
        DIE(sent < 0, "send_all");
    }

    // eliberez spatiul pentru vectorul de poll
    free(*poll_fds);

    // eliberez spatiul pentru vectorul de subscriberi
    for (int i = 0; i < num_subs; i++) {
        free((*subs)[i].ID);
        if ((*subs)[i].msg_num > 0) {
            free((*subs)[i].msg_index);
        }
    }
    free((*subs));

    // eliberez spatiul pentru vectorul de topicuri
    for (int i = 0; i < num_topics; i++) {
        free((*topics)[i].name);
        if ((*topics)[i].topic_subs_number > 0) {
            free((*topics)[i].subs_index);
            free((*topics)[i].SF);
        }
    }
    free(*topics);

    // eliberez spatiul pentru vectorul de mesaje
    for (int i = 0; i < num_messages; i++) {
        free((*messages)[i].packet->message);
        free((*messages)[i].packet);
    }
    free((*messages));
}

// adauga un nou mesaj in lista mesajelor ce trebuie trimise clientilor offline cu sf 1
void update_message_list (int *num_messages, struct message **messages, struct message *msg) {
    (*num_messages)++;
    (*messages) = realloc ((*messages), *num_messages * sizeof(struct message));
    DIE(!(*messages), "realloc");

    (*messages)[*num_messages - 1] = *msg;
}

// la reconectare trimite subscriberului mesajele ce au fost trimise cat timp era offline
// pe topicurile setate cu sf 1
void send_restant_messages (int *num_messages, struct message **messages, struct subscriber *sub,
                            int num_subs, struct subscriber **subs) {
    for (int i = 0; i < sub->msg_num; i++) {
        struct message *current_msg = ((*messages) + sub->msg_index[i]);
        send_message_packet(sub->fd,  *current_msg->packet);
        (current_msg->SF_remaining)--;

        if (current_msg->SF_remaining == 0) { // mesajul nu mai trebuie trimis nimanui, deci trebuie scos
            if (*num_messages == 1) { // daca e ultimul mesaj din lista
                *num_messages = 0;
                *messages = NULL;
                continue;
            }
            // mut elementul de pe ultima pozitie in locul celui care trebuie sters
            memcpy(*messages + sub->msg_index[i], *messages + *num_messages - 1, sizeof(struct message));

            // actualizeaza noua pozitie a elementului care era la finalul sirului
            replace(*num_messages - 1, sub->msg_index[i], num_subs, subs);
            (*num_messages)--;
            *messages = realloc ((*messages), *num_messages * sizeof(struct message));
            DIE(!messages, "realloc");
        }

    }

    sub->msg_index = NULL;
    sub->msg_num = 0;
}

// cauta in vectorii msg_index din fiecare vector si inlocuieste fiecare aparitie
// a lui index1 cu index2
void replace (int index1, int index2, int num_subs, struct subscriber **subs) {
    for (int i = 0; i < num_subs; i++) {
        struct subscriber *sub = (*subs) + i;
        if (sub->msg_num == 0) // vectorul de msg_index e gol
            continue;
        
        for (int j = 0; j < sub->msg_num; j++) {
            if (sub->msg_index[j] == index1) {
                sub->msg_index[j] = index2;
            }
        }
    }
}

// printeaza continutul listei de mesaje
void print_messages (int num_messages, struct message *messages) {
    printf("numarul de mesaje: %d\n", num_messages);
    printf("--------------------------------\n");
    for (int i = 0; i < num_messages; i++) {
        char *topic = calloc(51, 1);
        DIE(!topic, "calloc");

        memcpy(topic, messages[i].packet->message, 50);
        printf("topic: %s\nSF_remaining: %d\n", topic, messages[i].SF_remaining);
        printf("--------------------------------\n");
    }

    printf ("\n\n");
}

// printeaza numarul de mesaje restante si indexii acestora pentru un anumit sub
void print_restant_messages (struct subscriber *sub) {
    printf("user %s has %d restant messages\n", sub->ID, sub->msg_num);
    for (int i = 0; i < sub->msg_num; i++) {
        printf("%d ", sub->msg_index[i]);
    }

    printf("\n\n");
}