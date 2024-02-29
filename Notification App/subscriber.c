#include "utils/common.h"
#include "utils/helpers.h"

void run_client(int sockfd) {
  struct pollfd pfds[2];
  int nfds = 0;

  pfds[0].fd = STDIN_FILENO;
  pfds[0].events = POLLIN;
  nfds++;

  pfds[1].fd = sockfd;
  pfds[1].events = POLLIN;
  nfds++;


  while (1) {
    poll(pfds, nfds, -1);

    if ((pfds[0].revents & POLLIN) != 0) { // standard input
      manage_stdin_input(sockfd);
    } else if ((pfds[1].revents & POLLIN) != 0) { // received from server
      struct message_packet *msg = receive_message_packet(pfds[1].fd);
      print_message(msg);
    }
  }
}

int main(int argc, char *argv[]) {
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);

  int sockfd = -1;

  if (argc != 4) {
    perror("wrong number of arguments\n");
    return 1;
  }

  // Parsam port-ul ca un numar
  uint16_t port;
  int rc = sscanf(argv[3], "%hu", &port);
  if (rc != 1) {
    perror ("Invalid port");
  }


  // Obtinem un socket TCP pentru conectarea la server
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  DIE(sockfd < 0, "socket");

  // Completăm in serv_addr adresa serverului, familia de adrese si portul
  // pentru conectare
  struct sockaddr_in serv_addr;
  socklen_t socket_len = sizeof(struct sockaddr_in);

  memset(&serv_addr, 0, socket_len);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  rc = inet_pton(AF_INET, argv[2], &serv_addr.sin_addr.s_addr);
  DIE(rc <= 0, "inet_pton");

  // dezactivam nagle
  int nagle = 1;
  if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &nagle, sizeof(nagle)) < 0)
        perror("setsockopt(TCP_NODELAY) failed");

  // Ne conectăm la server
  rc = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  DIE(rc < 0, "connect");

  struct ID_packet subscriber_ID;
  subscriber_ID.len = strlen(argv[1]);
  subscriber_ID.ID = argv[1];

  // trimit pe rand lungimea id ului si dupa id ul
  send_all(sockfd, &subscriber_ID.len, sizeof(uint16_t));
  send_all(sockfd, subscriber_ID.ID, subscriber_ID.len);


  run_client(sockfd);

  // Inchidem conexiunea si socketul creat
  close(sockfd);

  return 0;
}
