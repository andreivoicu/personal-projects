#ifndef __COMMON_H__
#define __COMMON_H__

#include <stddef.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <netinet/tcp.h>

/* Dimensiunea maxima a mesajului */
#define MSG_MAXSIZE 1024

struct subscribe_packet {
  // 1 for subscribe, 0 for unsubscribe;
  int8_t subscribe;

  // subscription topic and length of topic
  uint16_t topic_len;
  char* topic; 

  // 1 or 0 for subscribe, -1 for unsubscribe
  int8_t SF;
};

struct ID_packet {
  // ID of the newly connected subscriber
  // this packet is sent only once, after tcp handshake
  uint16_t len;
  char* ID;
};

struct subscriber {
  char* ID;  // subscriber ID
  int fd;    // file descriptor for the socket, -1 if subscriber offline

  int msg_num;     // num of restant messages
  int* msg_index;  // index in message list for restant messages
                   // of a particular subscriber
};

struct message_packet {
  struct in_addr UDP_addr; // use with inet_ntoa
  unsigned short UDP_port;

  uint16_t message_len;
  char* message; 
};

struct message {
  struct message_packet *packet;
  short SF_remaining;
};

struct topic {
  char* name;

  short topic_subs_number;
  int* subs_index;   // index of subs vector
                     // 0 means the first elem in the subs vector, 3 means the 4th elem etc
  int8_t* SF;     // SF 1 or 0 for each subscriber in the subs vector 
};

int send_all(int sockfd, void *buff, size_t len);
int recv_all(int sockfd, void *buff, size_t len);

// client functions
int send_sub_packet (struct subscribe_packet sub_packet, int socketfd);
void manage_stdin_input (int socketfd);
struct message_packet* receive_message_packet (int fd);
void print_message (struct message_packet *message);

// server functions
int8_t get_stdin_input ();
struct subscriber* accept_sub_connection (int listenfd, int num_subs, struct subscriber *subs);
int sub_exists (char* ID, int num_subs, struct subscriber *subs);
void print_subs (int num_subs, struct subscriber *subs);
void close_sub_connection (int num_subs, struct subscriber **subs,
                        int *num_clients, struct pollfd **poll_fds, int closefd);
int update_subs_add (int num_subs, struct subscriber **subs, struct subscriber *new_sub,
                    int *num_messages, struct message **messages);
int update_poll_fds (int num_clients, struct pollfd **poll_fds, int newfd);

int get_TCP_socket (uint16_t port);
int get_UDP_socket (uint16_t port);
struct message_packet* read_UDP_message(int UDP_fd);
struct message* initialize_message (int UDP_fd);
char* get_topic (struct message* msg);
struct subscribe_packet* receive_subscriber_packet (int sockfd);
struct subscriber* get_subscriber_address (int fd, int num_subs, struct subscriber *subs);
struct topic* get_topic_address (int num_topics, struct topic *topics, char* name);
int8_t manage_subscribing (int *num_topics, struct topic **topics, int sub_index,
                            char* topic_name, int8_t SF);
int8_t manage_unsubscribing (int num_topics, struct topic *topics, int sub_index, char* topic_name,
                             int num_subs, struct subscriber *subs);
void manage_subscriber_packet (int sockfd, struct subscribe_packet *sub_packet, int *num_topics,
                                struct topic **topics, int num_subs, struct subscriber *subs);
void print_topics (int num_topics, struct topic *topics);
void send_message_packet (int fd, struct message_packet msg);
short manage_message_packet (int *num_topics, struct topic **topics, struct message msg,
                           int num_subs, struct subscriber *subs, int num_messages);
void close_server (struct pollfd **poll_fds, int num_subs, struct subscriber **subs,
                   int num_topics, struct topic **topics, int num_messages, struct message **messages);
void update_message_list (int *num_messages, struct message **messages, struct message *msg);
void send_restant_messages (int *num_messages, struct message **messages, struct subscriber *sub,
                            int num_subs, struct subscriber **subs);
void replace (int index1, int index2, int num_subs, struct subscriber **subs);
void print_messages (int num_messages, struct message *messages);
void print_restant_messages (struct subscriber *sub);
#endif
