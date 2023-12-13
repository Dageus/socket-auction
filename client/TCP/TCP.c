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


    if (response->file == NULL){
        /**
         * Enviar mensagem para o servidor
        */
        
        if ((send(fd, response->msg , MAX_TCP_NULL_FILE, 0)) == -1) {
            /*error*/
            fprintf(stderr, "Error sending message to server\n");
            return -1;
        }
            

        char buffer[MAX_TCP_NULL_FILERCV] ; // Buffer to store received data
        size_t buffer_read = 0; // Current buffer size

        if ((tcp_n = recv(fd, buffer, MAX_TCP_NULL_FILERCV, 0)) == -1) {
            /*error*/
            fprintf(stderr, "Error receiving message to server\n"); 
            return -1;
        } 

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

        char filesize_str[20];
        sprintf(filesize_str, "%ld", filesize);

        strcat(response->msg, filesize_str);
        strcat(response->msg, " ");

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

    

        char temp[TRANSMISSION_RATE];
        tcp_n = fread(temp, 1, sizeof(temp) - strlen(response->msg) - 1, file);
        if (tcp_n == -1) {
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

        while ((tcp_n = fread(tcp_buffer, 1, sizeof(tcp_buffer), file)) > 0) {
            if (send(fd, tcp_buffer, tcp_n, 0) == -1) {
                fprintf(stderr, "Error sending data\n");
                fclose(file);
                close(fd);
                return -1;
            }
        }

        if ((recv(fd, tcp_buffer, TRANSMISSION_RATE, 0)) == -1) {
            /*error*/
            fprintf(stderr, "Error receiving message to server\n");
            return -1;
        } 

        // Close the file
        fclose(file);
    } else if (response->file->code == FILE_TO_BE_RECEIVED) {
        /**
         * Receber ficheiro do servidor e escrever para o disco
        */

       

        if ((send(fd, response->msg , MAX_TCP_NULL_FILE, 0)) == -1) {
            /*error*/
            fprintf(stderr, "Error sending message to server\n");
            return -1;
        }
           
        // check if file exists by checking the status of the received message

        char* token = strtok(response->msg, " ");

        if(strcmp(token = strtok(NULL, " "), "OK") == 0) {
            // file exists
            char* filename = strtok(NULL, " ");
            char* filesize_str = strtok(NULL, " ");

            size_t filesize = atoi(filesize_str);

            FILE *file = fopen(filename, "wb");
            if (!file) {
                fprintf(stderr, "Error opening file\n");
                freeaddrinfo(tcp_res);
                close(fd);
                return -1;
            }

            size_t bytes_received = 0;
            while (bytes_received < filesize) {
                tcp_n = recv(fd, tcp_buffer, sizeof(tcp_buffer), 0);
                if (tcp_n == -1) {
                    fprintf(stderr, "Error receiving data\n");
                    fclose(file);
                    freeaddrinfo(tcp_res);
                    close(fd);
                    return -1;
                }

                bytes_received += tcp_n;

                if (fwrite(tcp_buffer, 1, tcp_n, file) != tcp_n) {
                    fprintf(stderr, "Error writing file\n");
                    fclose(file);
                    freeaddrinfo(tcp_res);
                    close(fd);
                    return -1;
                }
            }

            fclose(file);
        } else {
            // file does not exist
            fprintf(stderr, "Error: file does not exist\n");
            freeaddrinfo(tcp_res);
            close(fd);
            return -1;
        }

    }

    /* Desaloca a memória da estrutura `tcp_res` e fecha o socket */
    freeaddrinfo(tcp_res);
    close(fd);

    return 1;
}