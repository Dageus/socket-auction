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
#include "constants.h"
#include <signal.h>
#include "UDP/UDP.h"


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

    
        
    
}




void process_command(int fd, char *response, struct sockaddr_in addr, socklen_t addrlen) {

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
        int udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (udp_fd == -1) {
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

            

            // send reply to client

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