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

#include "bid/bid.h"
#include "open/open.h"
#include "close/close.h"
#include "show_asset/show_asset.h"

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

int read_word(int fd, char *buffer, int size) {
    int i = 0;
    while (i < size)
    {
        if (read(fd, buffer + i, sizeof(char)) == -1)
        {
            printf("Error while reading from socket.\n");
            buffer = NULL;
            return -1;
        }
        if (buffer[i] == ' ' || buffer[i] == '\n')
        {
            memset(buffer + i, '\0', 1);
            printf("read_word: %s\n", buffer);
            break;
        }
        i++;
    }

    return 0;
}

int TCP_cmd(char* cmd){
    int i;
    for (i = 0; i < 6; i++) {
        if (strcmp(cmd, TCP_commands[i]) == 0)
            return 1;
    }
    return 0;
}

int null_file_message(int fd, char* msg){

    printf("msg: %s, with len: %ld\n", msg, strlen(msg));

    if (send(fd, msg , strlen(msg), 0) == -1) {
        /*error*/
        fprintf(stderr, "Error sending message to server\n");
        return -1;
    }

    char buffer[MAX_TCP_NULL_FILERCV] ; // Buffer to store received data

    if ((tcp_n = read(fd, buffer, MAX_TCP_NULL_FILERCV)) == -1) {
        /*error*/
        fprintf(stderr, "Error receiving message to server\n"); 
        return -1;
    } 

    printf("received %ld bytes\n", tcp_n);

    printf("received message from server: %s\n", buffer);

    return 1;
}

int open_auction_message(int fd, char* filename, char* msg){

    struct stat st;
    size_t filesize;

    if (stat(filename, &st) == 0) {
        filesize = st.st_size;
    } else {
        fprintf(stderr, "Error getting file size\n");
        freeaddrinfo(tcp_res);
        close(fd);
        return -1;
    }

    char filesize_str[10];
    sprintf(filesize_str, "%ld", filesize);

    strcat(msg, filesize_str);
    strcat(msg, " ");
    msg[strlen(msg)] = '\0';

    printf("msg: %s|\n", msg);

    // Send the file data

    if ((write(fd, msg, strlen(msg))) == -1) {
        fprintf(stderr, "Error sending data\n");
        freeaddrinfo(tcp_res);
        close(fd);
        return -1;
    }

    ssize_t bytes_sent;
    size_t total_bytes_sent = 0;

    printf("sending file...\n");

    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error opening file\n");
        freeaddrinfo(tcp_res);
        close(fd);
        return -1;
    }

    while ((tcp_n = fread(tcp_buffer, 1, sizeof(tcp_buffer), file)) > 0) {
        printf("reading more data...\n");
        if ((bytes_sent = write(fd, tcp_buffer, tcp_n)) == -1) {
            fprintf(stderr, "Error sending data\n");
            fclose(file);
            close(fd);
            return -1;
        }
        printf("wrote %ld bytes\n", bytes_sent);
        total_bytes_sent += bytes_sent;
    }

    printf("total_bytes_sent: %ld\n", total_bytes_sent);

    ssize_t bytes_read;
    if ((bytes_read = read(fd, tcp_buffer, TRANSMISSION_RATE)) == -1) {
        /*error*/
        fprintf(stderr, "Error receiving message from server\n");
        return -1;
    } 

    printf("received %ld bytes\n", bytes_read);
    tcp_buffer[bytes_read] = '\0';
    printf("received message from server: %s\n", tcp_buffer);

    printf("closing file\n");
    // Close the file
    fclose(file);
}

int receive_asset_message(int fd, char* msg){

    printf("msg: %s, with len: %ld\n", msg, strlen(msg));
    
    if ((send(fd, msg , strlen(msg), 0)) == -1) {
        /* error */
        fprintf(stderr, "Error sending message to server\n");
        return -1;
    }
        
    // check if file exists by checking the status of the received message

    char response[4];
    read_word(fd, response, 4);

    printf("response: %s\n", response);

    char status[4];
    read_word(fd, status, 4);

    printf("status: %s\n", status);

    if (strcmp(status, "NOK") == 0) {
        fprintf(stderr, "Error: server could not complete request\n");
        freeaddrinfo(tcp_res);
        close(fd);
        return -1;
    }

    printf("reading filename and filesize...\n");

    char fname[MAX_FILENAME_LEN + 1];
    read_word(fd, fname, MAX_FILENAME_LEN);
    printf("fname: %s\n", fname);

    char filesize_str[10];
    read_word(fd, filesize_str, 10);
    printf("filesize_str: %s\n", filesize_str);

    FILE *file = fopen(fname, "wb");

    if (!file) {
        fprintf(stderr, "Error opening file\n");
        freeaddrinfo(tcp_res);
        close(fd);
        return -1;
    }

    int total_bytes_received = 0;
    int bytes_received;


    while ( (size_t) total_bytes_received < atoi(filesize_str)) {
        
        bytes_received = read(fd, tcp_buffer, TRANSMISSION_RATE);
        
        if (bytes_received < 0){
            perror("Error reading from TCP socket");
            fprintf(stderr, "Error reading from TCP socket\n");
            fclose(file);
            close(fd);
            return -1;
        }

        printf("\n");
        printf("received %d bytes\n", bytes_received);
        printf("\n");

        total_bytes_received += bytes_received;

        printf("total_bytes_received: %d\n", total_bytes_received);

        if (fwrite(tcp_buffer, 1 , bytes_received, file) != (size_t) bytes_received){
            fprintf(stderr, "Error writing file\n");
            fclose(file);
            close(fd);
            return -1;
        
        } // Write the received bytes to the file
        
    }

    printf("\n");
    printf("received %d bytes\n", total_bytes_received);
    printf("\n");

    fclose(file);

    fprintf(stdout, "File %s received\n", fname);
}

/**
 * Receives a TCP_response struct and sends it to the server
 * Might send a file or a message
 * 
 * @param response
 * 
 * @return 1 if successful, -1 otherwise
*/
int send_TCP(TCP_response* response, char* ip, char* port){

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

    tcp_errcode = getaddrinfo(ip, port, &tcp_hints, &tcp_res);
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


    if (response->file == NULL) {
        
        if (null_file_message(fd, response->msg) == -1){
            fprintf(stderr, "[ERROR]: null file message\n");
            return -1;
        }

    } else if (response->file->code == FILE_TO_BE_SENT) {

        /**
         * Ler e enviar o ficheiro para o servidor
        */

        if (open_auction_message(fd, response->file->filename, response->msg) == -1){
            fprintf(stderr, "[ERROR]: open auction message\n");
            return -1;
        }

        
    } else if (response->file->code == FILE_TO_BE_RECEIVED) {
        
        // Receber ficheiro do servidor e escrever para o disco

        if (receive_asset_message(fd, response->msg) == -1){
            fprintf(stderr, "[ERROR]: receive asset message\n");
        }
    }

    /* Desaloca a memória da estrutura `tcp_res` e fecha o socket */
    freeaddrinfo(tcp_res);
    close(fd);

    return 1;
}