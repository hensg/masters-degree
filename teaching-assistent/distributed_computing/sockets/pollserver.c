// pollserver.c -- a multiperson chat server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

#define PENDING_CONNECTIONS 40

int start_server(char* port) {
    int listener;
    struct addrinfo hints, *ai, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // escolhe o disponível - ipv4/ipv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE; // automaticamente pegue o ip localhost

    // função utilitára que preenche o *ai(addrinfo) baseado nas dicas/hints
    getaddrinfo(NULL, port, &hints, &ai); 

    // obtem o socket
    listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

    int yes = 1;
    
    // ## O que essa configuração faz? Que erro ele resolve/contorna?
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    // liga o socket no endereço:porta
    bind(listener, p->ai_addr, p->ai_addrlen);

    // já temos o socket, pode tirar o ai da memória
    freeaddrinfo(ai);

    // começa a ouvir o socket
    listen(listener, PENDING_CONNECTIONS);
    return listener;
}

// Adiciona o novo file descriptor ao array PFDS (Poll File DescriptorS)
void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size) {
    if (*fd_count == *fd_size) { // se o array encheu
        *fd_size *= 2; // dobre o tamaho dele
        *pfds = realloc(*pfds, sizeof(*pfds) * (*fd_size));
    }

    (*pfds)[*fd_count].fd = newfd;
    (*pfds)[*fd_count].events = POLLIN; 
    (*fd_count)++;
}

// remove o FD da posição i no array pdfs
void del_from_pfds(struct pollfd pfds[], int i, int *fd_count) {
    pfds[i] = pfds[*fd_count - 1];
    (*fd_count)--;
}

// umaa função utilitária que converte pro tipo certo
// baseado na família - ipv4 ou ipv6
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char* argv[]) {

    int listener, newfd;
    struct sockaddr_storage remoteaddr;
    socklen_t addrlen;
    char buf[256];
    char remoteIP[INET6_ADDRSTRLEN];

    int fd_count = 0;
    int fd_size = 5;
    struct pollfd *pfds = malloc(sizeof *pfds * fd_size);

    listener = start_server();

    pfds[0].fd = listener;
    pfds[0].events = POLLIN;
    fd_count = 1;

    for (;;) {
        int poll_count = poll(pfds, fd_count, -1);
        for (int i = 0; i < fd_count; i++) {
            if (pfds[i].revents & POLLIN) { //ready to read?

                if (pfds[i].fd == listener) {
                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener, (struct sockaddr*)&remoteaddr, &addrlen);
                    add_to_pfds(&pfds, newfd, &fd_count, &fd_size);
                    printf("pollserver: new connection from %s on socket %d\n",
                            inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr*)&remoteaddr),
                                remoteIP, INET6_ADDRSTRLEN), newfd);

                } else {
                    int nbytes = recv(pfds[i].fd, buf, sizeof buf, 0);
                    int sender_fd = pfds[i].fd;

                    if (nbytes <= 0) {
                        close(pfds[i].fd);
                        del_from_pfds(pfds, i, &fd_count);
                    } else {
                        for (int j = 0; j < fd_count; j++) {
                            int dest_fd = pfds[j].fd;
                            if (dest_fd != listener && dest_fd != sender_fd) {
                                send(dest_fd, buf, nbytes, 0);
                            }
                        }
                    }
                }
            }            
        }
    }
}
