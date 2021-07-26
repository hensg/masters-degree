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

/*
 * Function: start_server
 * ----------------------------
 * Inicializa um socket de servidor retornando um socket no estado listening
 * 
 * port: a porta para inicializar o servidor
 * returns: um socket no estado listening 
 */
int start_server(char* port) {
    int listener;
    struct addrinfo hints, *ai, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // escolhe o disponível - ipv4/ipv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE; // automaticamente pegue o ip localhost

    // função utilitára que preenche o *ai(addrinfo) baseado nas dicas/hints
    int err = getaddrinfo(NULL, port, &hints, &ai); // FIXME: tratar erro aqui

    // obtem o socket
    listener = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol); // FIXME: tratar erro aqui

    int yes = 1;
    // ##_1_## O que essa configuração faz? Que erro ele resolve/contorna?
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    // ##_2__## qual o tipo de erro que pode acontecer aqui?
    // liga o socket no endereço:porta
    err = bind(listener, ai->ai_addr, ai->ai_addrlen); // FIXME: tratar erro aqui

    // já temos o socket, pode tirar o ai da memória
    freeaddrinfo(ai);

    // começa a "escutar" o socket
    listen(listener, PENDING_CONNECTIONS); // FIXME: tratar erro aqui
    return listener;
}

/*
 * Function: add_to_pfds
 * ---------------------
 * Adiciona um novo file descriptor no array, se o mesmo não tiver espaço na
 * memória suficiente para caber este novo fd, dobre o espaço de memória do array
 *
 * pfds[]: array de pollfd
 * newfd: o novo fd que se conectou ao servidor
 * fd_count: quantidade de pollfds no array
 * fd_size: tamanho do array
 */
void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size) {
    if (*fd_count == *fd_size) { // se o array encheu
        *fd_size *= 2; // dobre o tamaho dele
        *pfds = realloc(*pfds, sizeof(**pfds) * (*fd_size));
    }

    (*pfds)[*fd_count].fd = newfd;
    (*pfds)[*fd_count].events = POLLIN; 
    (*fd_count)++;
}

/* Function: del_from_pfds
 * -----------------------
 * Remove um fd do array de pollfds
 *
 * pfds[]: array de pollfd
 * i: posição a ser removida
 * fd_count: qqauntidade de pollfds no array
 */
void del_from_pfds(struct pollfd pfds[], int i, int *fd_count) {
    pfds[i] = pfds[*fd_count - 1];
    (*fd_count)--;
}

/* Function: handle_pollin
 * ------------------------
 * Trata o evento POLLIN, ou seja, há dados para serem lidos que serão tratados
 * aqui.
 * 
 * A primeira parte é responsável por estabelecer novas conexões e as incluir no 
 * array de pollfds. Já a segunda, é quando foram recebidos alguns dados e os
 * mesmos devem ser encaminhados aos outros clientes.
 *
 * pfds: ponteiro para o array de pollfd
 * i: índice de qual o socket está no modo pronto para ser lido
 * listener: socket do servidor no estado listening
 * fd_count: quantidade de file descriptors no array
 * fd_size: tamanho atual do array
 *
 */
void handle_pollin(struct pollfd *pfds, int i, int listener, int *fd_count, int *fd_size) {
    
    // se o evento de entrada for no socket que está escutando conexões
    // então iremos executar a chamada de "aceite" e obter um novo socket
    // para o cliente
    if (pfds[i].fd == listener) {
        
        struct sockaddr_storage remoteaddr;
        socklen_t addrlen;
        addrlen = sizeof remoteaddr;

        //estabelece a conexão e retorna o novo socket para comunicação com o cliente
        // ##_3__## Você saberia explicar o que acontece nessa chamada de sistema para um socket TCP?
        int newfd = accept(listener, (struct sockaddr*)&remoteaddr, &addrlen);
        printf("Nova conexão estabelecida, novo fd=%d\n", newfd);

        // adiciona o socket ao array de sockets
        add_to_pfds(&pfds, newfd, fd_count, fd_size);

    } else {
    //senao, o evento está ocorrendo num socket com conexão já
    //estabelecida e podemos trocar dados através dele

        size_t buf_size = 256;
        char buf[buf_size]; // um buffer para armazenar os dados que chegaram no socket

        // ##_4__## O que acontece se a quantidade de dados no socket forem maior 
        // que o tamanho do buffer? O que você faria de diferente aqui?
        int nbytes = recv(pfds[i].fd, buf, buf_size, 0);
        int sender_fd = pfds[i].fd;

        if (nbytes > 0) { // foram recebidos alguns dados
            printf("%d bytes recebidos com a mensagem %s", nbytes, buf);

            for (int j = 0; j < *fd_count; j++) {
                // envie para todos os sockets que não são
                // nem quem enviou os dados e nem o socket "listener"

                int dest_fd = pfds[j].fd;
                if (dest_fd != listener && dest_fd != sender_fd) {
                    printf("Enviando mensagem para o client do socket %d\n", dest_fd);
                    send(dest_fd, buf, nbytes, 0);
                }
            }
        } else {
            // se o número de bytes que está no socket for negativo ou 0
            // é uma conexão sendo encerrada
            close(pfds[i].fd);
            del_from_pfds(pfds, i, fd_count);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./server <port>\n");
        exit(EXIT_FAILURE);
    }
    char* port = argv[1];

    int listener = start_server(port);
    printf("Listening..\n");

    // inicia um array com espaço para 5 file descriptors
    int fd_size = 5;
    struct pollfd *pfds = malloc(sizeof *pfds * fd_size);

    pfds[0].fd = listener; // inclui o listener nesse array
    pfds[0].events = POLLIN;
    int fd_count = 1;

    for (;;) {

        // chamada "poll", com o array e o número atual de sockets que se tem
        // essa chamada é bloqueante - execute "man poll" para mais detalhes no linux
        int poll_count = poll(pfds, fd_count, -1);

        for (int i = 0; i < fd_count; i++) {

            switch (pfds[i].revents)  {
                case 0:
                    break;
                case POLLIN: // há dados no socket "i" para serem lidos
                    handle_pollin(pfds, i, listener, &fd_count, &fd_size); // leitura do socket
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
                    exit(EXIT_FAILURE);
            }
        }
    }
}
