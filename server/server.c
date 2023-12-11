#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <bits/getopt_core.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "constants.h"
#include <signal.h>
#include "UDP/UDP.h"
#include "TCP/TCP.h"


int fd;

int verbose = FALSE;
char *port = DEFAULT_PORT;

struct addrinfo hints, *res;

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

typedef struct {
    char cmd[4];
    char *input;
} cmds;

void check_UDP_command(cmds command) {

    UDP_response* response = NULL;

    if(strcmp(command.cmd, "LIN") == 0){
        if(process_user_login(command.input, response) == -1)
            printf("Error in LIN command\n");
    } else if(strcmp(command.cmd, "LOU") == 0){
        if(process_user_logout(command.input, response) == -1)
            printf("Error in LOU command\n");
    } else if(strcmp(command.cmd, "UNR") == 0){
        if(process_unregister(command.input, response) == -1)
            printf("Error in UNR command\n"); 
    } else if(strcmp(command.cmd, "LMA") == 0){
        if(process_myauctions(command.input, response) == -1)
            printf("Error in LMA command\n");
    } else if(strcmp(command.cmd, "LMB") == 0){
        if(process_mybids(command.input, response) == -1)
            printf("Error in LMB command\n");
    } else if(strcmp(command.cmd, "LST") == 0){
        if(process_list(command.input, response) == -1)
            printf("Error in LST command\n");
    } else if(strcmp(command.cmd, "SRC") == 0){
        if(process_show_record(command.input, response) == -1)
            printf("Error in SRC command\n");
    } else {
        printf("Invalid command\n");
    }
    
        
    printf("UDP response: %s\n", response);

    
    
}

void check_TCP_command(cmds command){

    TCP_response* response = NULL;

    if(strcmp(command.cmd, "OPA") == 0){
        if(process_open_auction(command.input, response) == -1)
            printf("Error in OPA command\n");
    }else if(strcmp(command.cmd, "CLS") == 0){
        if(CLS(command) == -1)
            printf("Error in CLS command\n");
    }else if(strcmp(command.cmd, "SAS") == 0){
        if(SAS(command) == -1)
            printf("Error in SAS command\n");
    }else if(strcmp(command.cmd, "BID") == 0){
        if(process_bid(command.input, response) == -1)
            printf("Error in BID command\n");
    }

        
    printf("TCP response: %s\n", response);


}


void process_command(char *response) {

    cmds command;
    memcpy(command.cmd, response, 4); 
    command.cmd[4] = '\0'; 

    size_t input_length = strlen(response) - 4;
    command.input = (char* ) malloc((input_length + 1) * sizeof(char)); // Allocate memory for input
    if (command.input == NULL) {
        printf("Error allocating memory for input\n");
        exit(1);
    }
    memcpy(command.input, response + 4, input_length); 
    command.input[input_length] = '\0'; 

    if(UDP_cmd(command.cmd)){
        check_UDP_command(command);
    }
    else if (TCP_cmd(command.cmd)){
        check_TCP_command(command);
    }
    else {
        printf("Invalid command\n");
    }
}

void sigchld_handler(int signo) {
    (void)signo;
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void create_udp_socket(char buffer){
    
        // create UDP socket
        int fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (fd == -1) {
            printf("Error creating UDP socket\n");
            exit(1);
        }
    
        memset(&hints, 0, sizeof(&hints));
        hints.ai_family = AF_INET;      // IPv4
        hints.ai_socktype = SOCK_DGRAM;   // UDP socket
        hints.ai_flags = AI_PASSIVE;
    
        int errcode = getaddrinfo(NULL, port, &hints, &res);
        if ((errcode) != 0){
            /*error*/
            fprintf(stderr, "Error getting UDP address info\n");
            exit(1);
        }
    
        ssize_t n = bind(fd,res->ai_addr,res->ai_addrlen);
        if (n == -1){
            /*error*/ 
            fprintf(stderr, "Error binding UDP socket\n");
            exit(1);
        } 

        while (TRUE) {

            // read from UDP socket

            struct sockaddr_in addr;

            socklen_t addrlen = sizeof(addr);
            n = recvfrom(fd, buffer, 128, 0, (struct sockaddr*) &addr, &addrlen);
            if (n == -1){
                /*error*/
                exit(1);
            }
            

            // print received message
            write(1, "received: ", 10);
            write(1, buffer, n);

            // do work here

            // process command

            process_command(buffer);

            // send reply to client but put this part in a function

            n = sendto(fd, buffer, n, 0, (struct sockaddr*) &addr, addrlen);
            if (n == -1){
                /*error*/
                exit(1);
            }

        }    
}

void create_tcp_scoket(char buffer){

    // create TCP socket

    int tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_fd == -1) {
        printf("Error creating TCP socket\n");
        exit(1);
    }

    memset(&hints, 0, sizeof(&hints));
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

    if (listen(tcp_fd, 5) == -1){
        /*error*/
        fprintf(stderr, "Error listening TCP socket\n");
        exit(1);
    }

    while (TRUE) {
        struct sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);
        int newfd;
        
        // accept TCP connection
        if ((newfd = accept(fd, (struct sockaddr*) &addr, &addrlen)) == -1) {
            exit(1);
        }
        
        // fork process
        pid_t subprocess = fork();
        
        if (subprocess == -1) {
            printf("Error forking process\n");
            exit(1);
        } else if (subprocess == 0) {
            // child process
            
            // read from TCP socket
            ssize_t n = read(newfd, buffer, 128);
            if (n == -1) {
                exit(1);
            }
            
            write(1, "received: ", 10);
            write(1, buffer, n);
            
            // process commands here

            process_command(buffer);
            
            // send reply to client
            n = write(newfd, buffer, n);
            if (n == -1) {
                exit(1);
            }
            
            close(newfd);
            exit(0);
        } else {
            // parent process
            close(newfd);
        }
    }


}

int main(int argc, char** argv){

    // validate arguments
    validate_args(argc, argv);

    // what's gonna be read from the socket
    char buffer[128];

    int process = fork() ;

    if (process == 0 )
        create_udp_socket(buffer);     
    else 
        create_tcp_scoket(buffer);
    return 0;
}


// criar funcação para enviar pelos sockets (UDP e TCP), consoante os casos,
// tendo em conta o que se há memória alocada para a estrutura de resposta TCP e UDP
// Perguntar se vale a pena ter a estrutura de resposta UDP e TCP para todos os casos
// ou se é melhor usar a estrutura nos casos em que aloca memória para a estrutura
// provavelmente é melhor ter a estrutura de resposta para todos os casos
// perguntar a Jomi

//falta fazer nos outros comandos
// TCP e UDP a alteração de char** para UDP_response** e TCP_response**