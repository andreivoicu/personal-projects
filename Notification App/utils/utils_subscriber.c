#include "common.h"
#include "helpers.h"

// trimite cerere de subscribe/unsusbscribe la server
int send_sub_packet (struct subscribe_packet sub_packet, int socketfd) {
  int total_sent = 0;
  int sent;

  sent = send_all(socketfd, &(sub_packet.subscribe), sizeof(int8_t));
  DIE(sent < 0, "send_all");
  total_sent += sent;

  sent = send_all(socketfd, &sub_packet.topic_len, sizeof(int16_t));
  DIE(sent < 0, "send_all");
  total_sent += sent;

  sub_packet.topic[sub_packet.topic_len] = '\0';
  sent = send_all(socketfd, sub_packet.topic, sub_packet.topic_len);
  DIE(sent < 0, "send_all");  
  total_sent += sent;

  sent = send_all(socketfd, &(sub_packet.SF), sizeof(int8_t));
  DIE(sent < 0, "send_all");
  total_sent += sent;

  return total_sent;
}

// citeste de la stdin si creeaza pachetul de subscribe
// pe care il trimite la server
void manage_stdin_input (int socketfd) {
  char *buf = calloc(MSG_MAXSIZE + 1, 1);
  DIE(!buf, "calloc client");
  memset(buf, 0, MSG_MAXSIZE + 1);

  if(fgets(buf, MSG_MAXSIZE, stdin) && !isspace(buf[0])) {
    if (strcmp(buf, "exit\n") == 0) {
      exit(1);
    }

    int len = strlen(buf);
    buf[len--] = '\0'; // removing \n
    char *command = buf;
    int index = 0;

    struct subscribe_packet sub_packet;
    memset (&sub_packet, 0, sizeof(struct subscribe_packet));

    while (index < len && buf[index] != ' '){
      index++;
    }

    if (index == len || index == len - 1) {
      perror("invalid command or number of arguments\n");
      free(buf);
      exit(1);
    }

    buf[index] = '\0';

    char* topic = (buf + index + 1);
    char sf;
    if (strcmp(command, "subscribe") == 0) {
      sub_packet.subscribe = 1;

      index++;
      while (index < len && buf[index] != ' '){
        index++;
      }

      if (index == len || index == len - 1) {
        perror("invalid arguments\n");
        free(buf);
        return;
      }
      buf[index] = '\0';

      sub_packet.topic_len = strlen(topic);
      sub_packet.topic = calloc(strlen(topic) + 1, 1);
      memcpy(sub_packet.topic, topic, strlen(topic) + 1);

      sf = buf[index + 1];

      if (sf == 48) { // aka 0
        sub_packet.SF = 0;
      } else if (sf == 49) { // aka 1
        sub_packet.SF = 1;
      } else {
        perror("invalid parameter: SF only 0 and 1\n");
        free(buf);
        return;
      }
    } else if (strcmp(command, "unsubscribe") == 0) {
      sub_packet.subscribe = 0;

      topic = (buf + index + 1);

      sub_packet.topic_len = strlen(topic + 1);
      sub_packet.topic = topic;
      sub_packet.SF = -1;
    } else {
      perror("invalid command\n");
      free(buf);
      return;
    }

    int sent = send_sub_packet(sub_packet, socketfd);
    DIE(sent < 0, "send_sub_package");

    int8_t confirmation;

    int rc = recv_all(socketfd, &confirmation, sizeof(int8_t));
    DIE(rc < 0, "recv_all");

    if (confirmation == 1) {
      if (sub_packet.subscribe == 1) {
        printf("Subscribed to topic.\n");
      } else if (sub_packet.subscribe == 0) {
        printf("Unsubscribed from topic.\n");
      }
    } else if (confirmation == -1) {
      perror("Topic does not exist.\n");
    } else if (confirmation == -2) {
      perror("You are not subscribed to this topic\n");
    }
    /*
      confirmation:
        1 = succesfully subscribed/unsubscribed
        -1 = topic doesn't exist! (only on unsubscribed)
        -2 = topic exist but client was not subscribed
    */

    // Use send_all function to send the pachet to the server.
    // send_all(socketfd, &sent_packet, sizeof(sent_packet));

    free(buf);
  }
}

// primeste un pachet cu un mesaj UDP de la server
struct message_packet* receive_message_packet (int fd) {
    struct message_packet *msg = calloc (sizeof(struct message_packet), 1);
    DIE(!msg, "calloc");

    int recv;
    
    int8_t confirmation = -1;
    recv = recv_all(fd, &confirmation, sizeof(int8_t));
    if (confirmation == 0) {
		    free(msg);
        printf("Exiting...\n");
      	exit(1);
    }

    // receive ip address
    recv = recv_all(fd, &(msg->UDP_addr), sizeof(struct in_addr));
    DIE(recv < 0, "recv_all");

    // receive sender's port
    recv = recv_all(fd, &(msg->UDP_port), sizeof(unsigned short));
    DIE(recv < 0, "recv_all");

    // receive payload length
    recv = recv_all(fd, &(msg->message_len), sizeof(uint16_t));
    DIE(recv < 0, "recv_all");

    // allocate memory & receive payload
    msg->message = calloc (msg->message_len + 1, 1);
    DIE(!(msg->message), "calloc");
    recv = recv_all(fd, msg->message, msg->message_len);
    DIE(recv < 0, "recv_all");

    return msg;
}

// primeste un pachet cu mesaj UDP si il printeaza dupa formatul specificat
void print_message (struct message_packet *message) {
    char *topic = calloc(51, 1);
    unsigned int tip;
    DIE(!topic, "calloc");

    printf("%s:%d - ", inet_ntoa(message->UDP_addr), ntohs(message->UDP_port));

    memcpy(topic, message->message, 50);
    tip = (unsigned int)message->message[50];
    printf("%s - ", topic);

    if (tip == 0) {         // int
        printf("INT -");
        char sign = message->message[51];
        uint32_t value = ntohl(*(uint32_t*)(message->message + 52));

        if (sign == 1) { // negative number
            value *= -1;
        } else if (sign != 0) {
            perror("Sign Byte error\n");
        }

        printf(" %d\n", value);
    } else if (tip == 1) { // short real
		printf("SHORT_REAL -");
        float value = ntohs(*(uint32_t*)(message->message + 51)) / 100.0;
        printf(" %.2f\n", value);
    } else if (tip == 2) { // float
		printf("FLOAT -");
        char sign = message->message[51];
        float value = ntohl(*(uint32_t*)(message->message + 52));
        uint8_t exp10 = *(uint8_t*)(message->message + 56);
        int duplicate10 = exp10;
        while (duplicate10) {
            value /= 10;
            duplicate10--;
        }
        if (sign == 1) { // negative number
            value *= -1;
        } else if (sign != 0) {
            perror("Sign Byte error\n");
        }
        printf(" %.*f\n", exp10, value);
    } else if (tip == 3) { // string
		printf("STRING -");
        char* value = calloc(message->message_len - 51, 1);
        DIE(!value, "calloc");

        memcpy(value, message->message + 51, message->message_len - 52);
        printf(" %s\n", value);

        free(value);
    }
    free(topic);
}