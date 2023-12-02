#include "TCP.h"
#include "../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

const char *TCP_commands[] = {
    "open",
    "close",
    "show_asset",
    "sa",
    "bid",
    "b"
};

int tcp_errcode;
ssize_t tcp_n;
socklen_t addrlen;
struct addrinfo tcp_hints, *tcp_res;
struct sockaddr_in tcp_addr;
char tcp_buffer[128];


int TCP_cmd(char* cmd){
    int i;
    for (i = 0; i < 6; i++) {
        if (strcmp(cmd, TCP_commands[i]) == 0)
            return 1;
    }
    return -1;
}

int send_TCP(char* msg){
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        /*error*/
        fprintf(stderr, "Error creating socket\n");
        return -1;
    }

    memset(&tcp_hints, 0, sizeof tcp_hints);
    tcp_hints.ai_family = AF_INET;
    tcp_hints.ai_socktype = SOCK_STREAM; // TCP socket

    tcp_errcode = getaddrinfo(SERVER_IP, TEST_PORT, &tcp_hints, &tcp_res);
    if (tcp_errcode != 0) {
        /*error*/
		fprintf(stderr, "Error sending message to server\n");
        return -1;
    }

    /* Em TCP é necessário estabelecer uma ligação com o servidor primeiro (Handshake).
       Então primeiro cria a conexão para o endereço obtido através de `getaddrinfo()`. */
    tcp_n = connect(fd, tcp_res->ai_addr, tcp_res->ai_addrlen);
    if (tcp_n == -1) {
        /*error*/
		fprintf(stderr, "Error connecting to server\n");
        return -1;
    }


    if ()
    FILE *file = fopen("path/to/your/image.jpg", "rb");
    if (!file) {
        fprintf(stderr, "Error opening file\n");
        freeaddrinfo(tcp_res);
        close(fd);
        exit(EXIT_FAILURE);
    }

    char buffer[TRANSMISSION_RATE];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (send(fd, buffer, bytesRead, 0) == -1) {
            fprintf(stderr, "Error sending data\n");
            fclose(file);
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    // Close the file
    fclose(file);

    /* Lê 128 Bytes do servidor e guarda-os no buffer. */
    tcp_n = read(fd, tcp_buffer, 128);
    if (tcp_n == -1) {
        /*error*/
		fprintf(stderr, "Error receiving message to server\n");
        return -1;
    }

    printf("Received from server: %s", tcp_buffer);

    /* Desaloca a memória da estrutura `tcp_res` e fecha o socket */
    freeaddrinfo(tcp_res);
    close(fd);


    return 0;
}