#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <poll.h>
#include <pthread.h>
#include "tpool.h"

#define PENDING_CONNECTIONS 500

typedef struct poll_fds_itens {
    struct pollfd *pfds;
    int i;
    int fd_count;
    int fd_size;
} poll_fds_itens_t;

pthread_mutex_t mutex;

/*
 * Function: add_to_pfds
 * ---------------------
 * Add new file descriptor to the poll to be notified when ready-to-read,
 * also make sure that the poll has room for the new file descriptor
 *
 * newfd: new file descriptor to be included into poll
 * pfds: the poll file descriptors
 * fd_count: current count of fds
 * fd_size: current size of the poll 
 */
void add_to_pfds(int newfd, struct pollfd *pfds, int *fd_count, int *fd_size) {
    if (*fd_count == *fd_size) {
        *fd_size *= 2;
        pfds = realloc(pfds, sizeof(*pfds) * (*fd_size));
    }
    pfds[*fd_count].fd = newfd;
    pfds[*fd_count].events = POLLIN;
    (*fd_count)++;
};

void remove_from_pfds(struct pollfd *pfds, int i, int *fd_count) {
    if (i != (*fd_count)-1) {
        pfds[i] = pfds[(*fd_count)-1];
    }
    (*fd_count)--;
}

int receive_data(struct pollfd *pfds, int i, int *fd_count) {
    char data_buf[32];
    int nbytes;
    do { 
        nbytes = recv(pfds[i].fd, data_buf, sizeof(data_buf), 0);

        if (nbytes > 0) {
            printf("Got message: %s\n", data_buf);

        } else if (nbytes == -1) {
            perror("Socket receive error");
            return -1;

        } else if (nbytes == 0) {
            printf("Closing connection...\n");
            int err;
            if ((err = close(pfds[i].fd)) == -1) {
                perror("Failed to close socket"); 
                return -1;
            }
            remove_from_pfds(pfds, i, fd_count);
        }

    } while (nbytes > 0);

    return 0;
};

void handle_pollin(struct pollfd *pfds, int i, int listener, int *fd_count, int *fd_size) {

    if (pfds[i].fd == listener) {

        struct sockaddr_storage remote_addr;
        socklen_t addr_len = sizeof remote_addr;
        char buf[INET_ADDRSTRLEN];

        int new_fd = accept(listener, (struct sockaddr*)&remote_addr, &addr_len);
        if (new_fd == -1)  {
            perror("accept");
        } else {
            printf("New connection from: %s\n", 
                inet_ntop(AF_INET, 
                    &((struct sockaddr_in*)&remote_addr)->sin_addr, buf, addr_len));
            add_to_pfds(new_fd, pfds, fd_count, fd_size); 
        }

    } else {
        receive_data(pfds, i, fd_count);
    }
}

/*
 * Function: start_server
 * ------------------------ 
 * start server at given port returning a socket id 
 *
 * port: port number to bind the socket
 * returns: socket id where server is listening
 */
int start_server(char* port) {
    struct addrinfo hints, *servinfo, *servinfo_list;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // ipv4 or ipv6
    hints.ai_socktype = SOCK_STREAM;

    char addr_buffer[INET_ADDRSTRLEN];
    memset(addr_buffer, 0, sizeof(addr_buffer));
    int sockfd;

    int err;
    if ((err = getaddrinfo("localhost", port, &hints, &servinfo_list)) != 0) {
        perror("Error trying to get local IP");
        return -1;
    }

    for (servinfo = servinfo_list; servinfo != NULL; servinfo = servinfo->ai_next) {
        printf("Starting server at %s:%s\n", inet_ntop(servinfo->ai_family,
                    &((struct sockaddr_in*)servinfo->ai_addr)->sin_addr, addr_buffer, INET_ADDRSTRLEN), port);


        sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
        //int yes = 1;
        //setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if ((err = bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen)) != 0) {
            perror("Failed to bind address/port to socket");
            return -1;
        }
        freeaddrinfo(servinfo);
    }

    if ((err = listen(sockfd, PENDING_CONNECTIONS)) != 0) {
        perror("Failed to start listening...");
        return -1;
    }

    printf("Server listening...\n");
    return sockfd;
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./server <port>\n");
        return -1;
    }
    char* port = argv[1];

    int fd_count = 1;
    int fd_size = 4;
    struct pollfd *pfds = malloc(sizeof(*pfds) * (fd_size));

    int listener = start_server(port);
    pfds[0].fd = listener;
    pfds[0].events = POLLIN;

    while(1) {
        int poll_count = poll(pfds, fd_count, -1);
        if (poll_count == -1) {
            perror("Error at poll function");
            return -1;
        }
        for (int i = 0; i < fd_count; i++) {
            switch (pfds[i].revents)  {
                case 0:
                    break;
                case POLLIN:
                    handle_pollin(pfds, i, listener, &fd_count, &fd_size);
                    break;
                case POLLNVAL:
                case POLLPRI:
                case POLLOUT:
                case POLLERR:
                case POLLHUP:
                case POLLRDBAND:
                case POLLWRBAND:
                default:
                    printf("Unespected revents\n");
                    return -1;
            }
        }
    }
    return 0;
}
