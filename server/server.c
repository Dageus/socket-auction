#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <bits/getopt_core.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include "constants.h"
#include "UDP/UDP.h"
#include "TCP/TCP.h"


int aid = 0;

int verbose = FALSE;
char *port = "58000";

struct addrinfo hints, *res;

void sigchld_handler(int signo) {
    (void)signo;
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

// initialize variable in case of incomplete command

void validate_args(int argc, char** argv) {

    int opt;

    while ((opt = getopt(argc, argv, "v:p:")) != -1) {
        switch (opt) {
            case 'v':
                verbose = TRUE;
                break;
            case 'p':
                port = optarg;
                printf("port: %s\n", port);
                break;
            default:
                fprintf(stderr, "Usage: %s [-n ip] [-p port]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
}

void check_UDP_command(cmds command, int fd, struct sockaddr_in addr, socklen_t addrlen) {

    char* response = NULL;

    printf("Command: %s, with len: %ld\n", command.cmd, strlen(command.cmd));

    if(strcmp(command.cmd, "LIN") == 0){
        printf("LIN command\n");
        if(process_user_login(command.input, &response) == -1)
            printf("Error in LIN command\n");
    } else if(strcmp(command.cmd, "LOU") == 0){
        if(process_user_logout(command.input, &response) == -1)
            printf("Error in LOU command\n");
    } else if(strcmp(command.cmd, "UNR") == 0){
        if(process_unregister(command.input, &response) == -1)
            printf("Error in UNR command\n"); 
    } else if(strcmp(command.cmd, "LMA") == 0){
        if(process_myauctions(command.input, &response) == -1)
            printf("Error in LMA command\n");
    } else if(strcmp(command.cmd, "LMB") == 0){
        if(process_mybids(command.input, &response) == -1)
            printf("Error in LMB command\n");
    } else if(strcmp(command.cmd, "LST") == 0){
        if(process_list( &response) == -1)
            printf("Error in LST command\n");
    } else if(strcmp(command.cmd, "SRC") == 0){
        if(process_show_record(command.input, &response) == -1)
            printf("Error in SRC command\n");
    } else {
        printf("Invalid command\n");
    }
    
    printf("UDP response: %s\n", response);

    char* response_ptr = response;
    int response_len = strlen(response);
    int bytes_sent = 0;

    while (bytes_sent < response_len) {
        int current_chunk_size = response_len - bytes_sent < READ_WRITE_RATE ? response_len - bytes_sent : READ_WRITE_RATE;
        int n = sendto(fd, response_ptr, current_chunk_size, 0, (struct sockaddr*) &addr, addrlen);
        if (n == -1) {
            perror("Error sending response");
        }
        bytes_sent += n;
        response_ptr += n;
    }

    if (response != NULL)
        free(response);
}

void check_TCP_command(cmds command, int fd){

     TCP_response* response = NULL;

    /*

    ??????????????????????????????????????    
    ?? MY BROTHER IN CHRIST WHAT IS THIS??
        ?? SO QUERO DIZER QUE ESTOU A??
            ?? PERCEBER O AFAZER??
    ??????????????????????????????????????
    
    */

    if(strcmp(command.cmd, "OPA") == 0){
         if(process_open_auction(fd, aid) == -1)
             printf("Error in OPA command\n");
    // }else if(strcmp(command.cmd, "CLS") == 0){
    //      if(CLS(command) == -1)
    //          printf("Error in CLS command\n");
    // }else if(strcmp(command.cmd, "SAS") == 0){
    //     if(SAS(command) == -1)
    //         printf("Error in SAS command\n");
    // }else if(strcmp(command.cmd, "BID") == 0){
    //     if(process_bid(command.input, response) == -1)
    //         printf("Error in BID command\n");
    }

        
    printf("TCP response: %s\n", response->msg);


}

void create_udp_socket() {

    int fd, errcode;
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints, *res;
    struct sockaddr_in addr;
    char buffer[128];

    fd = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket

    if (fd == -1) {
        printf("Socket error.\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP socket
    // hints.ai_flags = AI_PASSIVE;

    errcode = getaddrinfo(NULL, port, &hints, &res);

    if (errcode != 0) {
        perror("Address error.");
        exit(1);
    }

    n = bind(fd, res->ai_addr, res->ai_addrlen);

    if (n == -1) {
        perror("Bind error.");
        exit(1);
    }

    while (TRUE) {

        // read from UDP socket

        addrlen = sizeof(addr);

        printf("Waiting for data...\n");

        n = recvfrom(fd, buffer, 128, 0, (struct sockaddr *)&addr, &addrlen);
        buffer[n] = '\0';
        buffer[n - 1] = '\0';


        if (n == -1) {
            exit(1);
        }

        printf("Received: %s\n", buffer);

        cmds command;
        memcpy(command.cmd, buffer, 4); 
        command.cmd[3] = '\0'; 

        if (strcmp(command.cmd, "LST") != 0){
            size_t input_length = strlen(buffer) - 4;
            command.input = (char* ) malloc((input_length + 1) * sizeof(char)); // Allocate memory for input
            if (command.input == NULL) {
                printf("Error allocating memory for input\n");
                exit(1);
            }
            memcpy(command.input, buffer + 4, input_length); 
            command.input[input_length] = '\0'; 
        }


        // process commands here

        check_UDP_command(command, fd, addr, addrlen);
    }    
}

void create_tcp_scoket(){

// create TCP socket

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    if (fd == -1) {
        printf("Error creating TCP socket\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_STREAM;   // TCP socket
    hints.ai_flags = AI_PASSIVE;

    int errcode = getaddrinfo(NULL, port, &hints, &res);
    if ((errcode) != 0){
        /*error*/
        fprintf(stderr, "Error getting TCP address info\n");
        exit(1);
    }

    ssize_t n = bind(fd,res->ai_addr,res->ai_addrlen);

    if (n == -1){
        /*error*/ 
        fprintf(stderr, "Error binding TCP socket\n");
        exit(1);
    }

    if (listen(fd, 5) == -1){
        /*error*/
        fprintf(stderr, "Error listening TCP socket\n");
        exit(1);
    }
    

    //  accept TCP connection
    if ((fd = accept(fd, (struct sockaddr*) &addr, &addrlen)) == -1) {
        exit(1);
    }

    // read from TCP socket first 3 bytes

    cmds command;
    n = read(fd, command.cmd, 4);
    if(n == -1){
        fprintf(stderr, "Error reading from TCP socket\n");
        exit(1);
    }
    command.cmd[3] = '\0';

    check_TCP_command(command, fd);

    

    close(fd);
    exit(0);

    


}

int main(int argc, char** argv){

    // validate arguments
    validate_args(argc, argv);    

    // int process = fork() ;

    // if (process == 0 )
        create_udp_socket();     
    // else 
    //     create_tcp_scoket(buffer);
    return 0;
}