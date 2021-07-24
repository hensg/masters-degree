#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

int connect_to(char* server_addr, char* port) {
    struct addrinfo hints, *servinfo;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int err;
    if ((err = getaddrinfo(server_addr, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "Error trying to get address info for server\n");
        return -1;
    }

    char buf[INET_ADDRSTRLEN];
    printf("Connecting to server at %s:%s\n",
            inet_ntop(AF_INET, &((struct sockaddr_in*)servinfo->ai_addr)->sin_addr, buf, sizeof(buf)), port);

    int sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol); 
    if ((err = connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen)) != 0) {
        fprintf(stderr, "Failed to connect to server: %s\n", gai_strerror(err));
        return -1;
    }

    freeaddrinfo(servinfo);
    printf("Connected\n");
    return sockfd;
};

int send_msg(int sockfd, char* msg) {
    int err;
    if ((err = send(sockfd, msg, sizeof(msg), 0)) < 0) {
        fprintf(stderr, "Failed to send msg to server, error: %s\n", gai_strerror(err));
        return -1;
    }
    return 0;
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: ./client <server_addr> <port>\n");
        return -1;
    }
    char* server_addr = argv[1];
    char* port = argv[2];

    int sockfd = connect_to(server_addr, port);

    char msg[1024];
    while(1) {
        printf("\nMessage to send: ");
        scanf("%s", msg);
        send_msg(sockfd, msg);
    }
    return 0;
}
