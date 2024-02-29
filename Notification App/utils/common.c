#include "common.h"

#include <sys/socket.h>
#include <sys/types.h>

int recv_all(int sockfd, void *buffer, size_t len) {

    size_t bytes_received = 0;
    size_t bytes_remaining = len;
    char *buff = buffer;

    while(bytes_remaining) {
        int current_received = recv(sockfd, buff, bytes_remaining, 0);
        if (current_received == 0)
            break;
        buff += current_received;
        bytes_received += current_received;
        bytes_remaining -= current_received;
    }

    return bytes_received;
}

int send_all(int sockfd, void *buffer, size_t len) {
    size_t bytes_sent = 0;
    size_t bytes_remaining = len;
    char *buff = buffer;

    while(bytes_remaining) {
        int current_sent = send(sockfd, buff, bytes_remaining, 0);
        if (current_sent == 0)
            break;
        buff += current_sent;
        bytes_sent += current_sent;
        bytes_remaining -= current_sent;
    }
    
    return bytes_sent;
}