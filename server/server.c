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
#include "constants.h"

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

int main(int argc, char** argv){

    // validate arguments
    validate_args(argc, argv);

    // what's gonna be read from the socket
    char buffer[128];

    // create and listen to the sockets (UDP and TCP)

    // create UDP socket
    int udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_fd == -1) {
        printf("Error creating UDP socket\n");
        return -1;
    }

    memset(&hints, 0, sizeof(&hints));
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_DGRAM;   // UDP socket
    hints.ai_flags = AI_PASSIVE;

    int errcode = getaddrinfo(NULL, port, &hints, &res);
    if ((errcode) != 0){
        /*error*/
        fprintf(stderr, "Error getting UDP address info\n");
        return -1;
    }

    ssize_t n = bind(fd,res->ai_addr,res->ai_addrlen);
    if (n == -1){
        /*error*/ 
        fprintf(stderr, "Error binding UDP socket\n");
        return -1;
    } 
    
    // main code
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

        // ...

        // send reply to client

        n = sendto(fd, buffer, n, 0, (struct sockaddr*) &addr, addrlen);
        if (n == -1){
            /*error*/
            exit(1);
        }
        
        // -------------------------------------------------------------------------- 

        // read from TCP socket

        addrlen = sizeof(addr);

        int newfd;

        if (newfd = accept(fd, (struct sockaddr*) &addr, &addrlen) == -1){
            /*error*/
            exit(1);
        }

        n = read(newfd, buffer, 128);

        if( n== -1){
            /*error*/
            exit(1);
        } 

        write(1, "received: ", 10);
        write(1, buffer, n);
        
        n = write(newfd, buffer, n);
        if (n == -1){
            /*error*/
            exit(1);
        } 

        close(newfd);

    }

    return 0;
}