#include "utils/common.h"
#include "utils/helpers.h"

#define MAX_CONNECTIONS 32

void run_server(int TCP_fd, int UDP_fd) {
  // poll vector
	struct pollfd *poll_fds = calloc(3, sizeof(struct pollfd));
	int num_clients = 3;
	
  // subscriber vector
  struct subscriber *subs = NULL;
  int num_subs = 0;

  // topics vector
  struct topic *topics = NULL;
  int num_topics = 0;

  // in vectoru asta se afla doar mesajele ce trebuie trimise catre subs care sunt offline cu sf 1
  struct message *messages = NULL;
  int num_messages = 0;

  int rc;

	// Setam socket-ul listenfd pentru ascultare
	rc = listen(TCP_fd, MAX_CONNECTIONS); // TODO verificat daca pot fara limita la conexiuni
	DIE(rc < 0, "listen");

	// descriptorul pentru TCP
	poll_fds[0].fd = TCP_fd;
  poll_fds[0].events = POLLIN;
  poll_fds[0].revents = 0;

  // descriptorul pentru UDP
  poll_fds[1].fd = UDP_fd;
  poll_fds[1].events = POLLIN;
  poll_fds[1].revents = 0;

  // se adauga descriptorul pentru stdin
  poll_fds[2].fd = STDIN_FILENO;
  poll_fds[2].events = POLLIN;
  poll_fds[2].revents = 0;

	while (1) {

    rc = poll(poll_fds, num_clients, -1);
    DIE(rc < 0, "poll");

    for (int i = 0; i < num_clients; i++) {
      if (poll_fds[i].revents & POLLIN) {

        if (poll_fds[i].fd == TCP_fd) { // cerere pe socketul cu listen
          struct subscriber *new_sub = accept_sub_connection(TCP_fd, num_subs, subs);
          if (!(new_sub->ID)) {
            int8_t confirmation = 0;
            int sent = send_all(new_sub->fd, &confirmation, sizeof(int8_t));
            close(new_sub->fd);
            free(new_sub);
            
            continue;
          }
          num_subs = update_subs_add(num_subs, &subs, new_sub, &num_messages, &messages);

          num_clients = update_poll_fds(num_clients, &poll_fds, new_sub->fd);
        
        } else if (poll_fds[i].fd == UDP_fd) {
          struct message *msg = initialize_message(UDP_fd);

          short sf_num = manage_message_packet(&num_topics, &topics, *msg, num_subs, subs, num_messages);
          
          if (sf_num == 0) {
            free(msg);
            continue;
          }
          msg->SF_remaining = sf_num;

          // functie care adauga msg in lista de mesaje
          update_message_list(&num_messages, &messages, msg);
        } else if (poll_fds[i].fd == STDIN_FILENO) { // input de la tastatura
          int code = get_stdin_input ();
          if (code == 0) {
            close_server (&poll_fds, num_subs, &subs, num_topics, &topics, num_messages, &messages);
            exit(1);
          }
        } else { // s-au primit date pe un socket de client
          struct subscribe_packet* sub_packet = receive_subscriber_packet(poll_fds[i].fd);

          if (!sub_packet) { // clientul a inchis conexiunea
            close_sub_connection(num_subs, &subs, &num_clients, &poll_fds, poll_fds[i].fd);
            continue;
          }
          // clientul a trimis date
          manage_subscriber_packet(poll_fds[i].fd, sub_packet, &num_topics, &topics, num_subs, subs);
        }
      }
    }
  }
}

int main(int argc, char *argv[]) {
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);

  if (argc != 2) {
    printf("\n Usage: %s <ip> <port>\n", argv[0]);
    return 1;
  }

  // Parsam port-ul ca un numar
  uint16_t port;
  int rc = sscanf(argv[1], "%hu", &port);
  DIE(rc != 1, "Given port is invalid");

  // obtinem in socket tcp pentru a accepta legaturi de la clienti
  int TCP_fd = get_TCP_socket(port);
  int UDP_fd = get_UDP_socket(port);

  run_server(TCP_fd, UDP_fd);

  // Inchidem fd urile
  close(TCP_fd);
  close(UDP_fd);

  return 0;
}
