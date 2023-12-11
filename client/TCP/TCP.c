#include "TCP.h"
#include "../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
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
char tcp_buffer[TRANSMISSION_RATE];

/**
 * Checks if the command is valid for TCP
 * 
 * @param cmd
 * 
 * @return 1 if valid, -1 otherwise
*/
int TCP_cmd(char* cmd){
    int i;
    for (i = 0; i < 6; i++) {
        if (strcmp(cmd, TCP_commands[i]) == 0)
            return 1;
    }
    return -1;
}

/**
 * Receives a TCP_response struct and sends it to the server
 * Might send a file or a message
 * 
 * @param response
 * 
 * @return 1 if successful, -1 otherwise
*/
int send_TCP(TCP_response* response){

    /**
     * Abrir o socket TCP
    */
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

    ssize_t bytesRead;

    if (response->file == NULL){
        /**
         * Enviar mensagem para o servidor
        */

        tcp_n = send(fd, response->msg, strlen(response->msg), 0);   
        if (tcp_n == -1) {
            /*error*/
            fprintf(stderr, "Error sending message to server\n");
            return -1;
        }

        bytesRead = recv(fd, tcp_buffer, TRANSMISSION_RATE, 0);
        if (bytesRead == -1) {
            /*error*/
            fprintf(stderr, "Error receiving message to server\n");
            return -1;
        }

        printf("Received from server: %s", tcp_buffer);

    } else if (response->file->code == FILE_TO_BE_SENT) {
        /**
         * Ler e enviar o ficheiro para o servidor
        */

        struct stat st;
        size_t filesize;

        if (stat(response->file->filename, &st) == 0) {
            filesize = st.st_size;
        } else {
            fprintf(stderr, "Error getting file size\n");
            freeaddrinfo(tcp_res);
            close(fd);
            return -1;
        }

        strcat(response->msg, filesize);
        strcat(response->msg, " ");

        // ^^^ mensagem feita, falta escrever a data para o socket ^^^
        
        tcp_n = send(fd, response->msg, strlen(response->msg), 0);
        if (tcp_n == -1) {
            /*error*/
            fprintf(stderr, "Error sending message to server\n");
            
            
        }


        /**
         * Abrir o ficheiro e enviar para o servidor
        */
        FILE *file = fopen(response->file->filename, "rb");
        if (!file) {
            fprintf(stderr, "Error opening file\n");
            freeaddrinfo(tcp_res);
            close(fd);
            return -1;
        }

        // * fill the rest of the message with the file data

        char temp[TRANSMISSION_RATE];
        bytesRead = fread(temp, 1, sizeof(temp) - strlen(response->msg) - 1, file);
        if (bytesRead == -1) {
            fprintf(stderr, "Error reading file\n");
            fclose(file);
            freeaddrinfo(tcp_res);
            close(fd);
            return -1;
        }

        sprintf(tcp_buffer, "%s%s", response->msg, temp); // ??????????

        // Send the file data
        if (send(fd, tcp_buffer, strlen(tcp_buffer), 0) == -1) {
            fprintf(stderr, "Error sending data\n");
            fclose(file);
            freeaddrinfo(tcp_res);
            close(fd);
            return -1;
        }

        while ((bytesRead = fread(tcp_buffer, 1, sizeof(tcp_buffer), file)) > 0) {
            if (send(fd, tcp_buffer, bytesRead, 0) == -1) {
                fprintf(stderr, "Error sending data\n");
                fclose(file);
                close(fd);
                return -1;
            }
        }

        // Close the file
        fclose(file);
    } else if (response->file->code == FILE_TO_BE_RECEIVED) {
        /**
         * Receber ficheiro do servidor e escrever para o disco
        */
    }

    /* Desaloca a memória da estrutura `tcp_res` e fecha o socket */
    freeaddrinfo(tcp_res);
    close(fd);

    return 1;
}