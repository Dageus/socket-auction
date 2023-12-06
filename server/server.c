/**
 * 
 * Estrutura de quando um cliente se liga ao servidor:
 *  - O cliente envia uma mensagem ao servidor
 *  - criar (uid)_pass.txt
 *  - criar (uid)_login.txt
 * 
 * ESTES FICHEIROS SAO TEMPORARIOS E SO EXISTEM ENQUANTO O USER ESTIVER LOGGED IN
 * 
 * SEMPRE QUE O UTILIZADOR VOLTA A ENTRAR È CRIADO UM NOVO FICHEIRO PASS E LOGIN
 * 
 * 
*/



#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

#define TRUE 1
#define FALSE 0
#define AS_IP "tejo.tecnico.ulisboa.pt"
#define DEFAULT_PORT "58090"

int fd;

int verbose = FALSE;
char *port = DEFAULT_PORT;

struct addrinfo hints, *res;

// initialize variable in case of incomplete command

;

int validate_args(int argc, char** argv) {

    if (argc == 1) {
        // run AS on localhost
        // no port needed to be specified

        printf("Running AS on localhost\n");
    } else if (argc == 3) {
        // run AS on specified port
        // no port needed to be specified

        if (strcmp(argv[1], "-p") != 0) {
            // error
            // incomplete command, missing -n
            exit(1);
        }

        port = argv[2];

        printf("Running AS on specified port: %s\n", port);
    } else if (argc >= 4) {
        // run AS on specified IP
        // run AS on specified port

        if (strcmp(argv[1], "-p") != 0) {
            // error
            // incomplete command, missing -p

            printf("Error: missing -p argument\n");

            exit(1);
        } else if (strcmp(argv[3], "-v") != 0) {
            // error
            // incomplete command, missing -v

            printf("Error: missing -v argument\n");

            exit(1);
        }

        port = argv[2];
        verbose = TRUE;

        printf("Running AS on specified port: %s\n", port);
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
        exit(1);
    }

    memset(&hints, 0, sizeof(&hints));
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_DGRAM;   // UDP socket
    hints.ai_flags = AI_PASSIVE;

    int errcode = getaddrinfo(NULL, port, &hints, &res);
    if ((errcode) != 0){
        /*error*/
        exit(1);
    }

    ssize_t n = bind(fd,res->ai_addr,res->ai_addrlen);
    if (n == -1){
        /*error*/ 
        exit(1);    
    } 

    // create TCP socket
    int tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_fd == -1) {
        printf("Error creating TCP socket\n");
        exit(1);
    }

    if (listen(fd, 5) == -1){
        /*error*/
        exit(1);
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