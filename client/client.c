#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include "commands.h"

#define TRUE 1

#define SERVER_IP "tejo.tecnico.ulisboa.pt"
#define TEST_PORT 58011
#define DEFAULT_PORT 58090

#define LOCAL_SERVER_IP "localhost"


int fd,errcode;
ssize_t n;
socklen_t addrlen;
struct addrinfo hints, *res;
struct sockaddr_in addr;
char buffer[128];


// initialize default values in case of incomplete command

char *ip = LOCAL_SERVER_IP;
int port = DEFAULT_PORT;

int validate_args(int argc, char** argv) {

    int opt;

    while ((opt = getopt(argc, argv, "n:p:")) != -1) {
        switch (opt) {
            case 'n':
                char *ip = optarg;
                printf("ip: %s\n", ip);
                break;
            case 'p':
                port = atoi(optarg);
                printf("port: %d\n", port);
                break;
            default:
                fprintf(stderr, "Usage: %s [-n ip] [-p port]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (argc == 1)
        printf("ip: %s\n", ip);
}

void process_cmd(char* input){
    if (strcmp(input, "login") == 0) {
        // process_login();
    } else if (strcmp(input, "logout") == 0) {
        // process_logout();
    } else if (strcmp(input, "unregister") == 0) {
        // process_unregister();
    } else if (strcmp(input, "exit") == 0) {
        // process_exit();
    } else if (strcmp(input, "open") == 0) {
        // process_open();
    } else if (strcmp(input, "close") == 0) {
        // process_close();
    } else if (strcmp(input, "myauctions") == 0 || strcmp(input, "ma") == 0) {
        // process_auctions();
    } else if (strcmp(input, "mybids") == 0 || strcmp(input, "mb") == 0) {
        // process_bids();
    } else if (strcmp(input, "list") == 0 || strcmp(input, "l") == 0) {
        // process_list();
    } else if (strcmp(input, "show_asset") == 0 || strcmp(input, "sa") == 0) {
        // process_asset();
    } else if (strcmp(input, "bid") == 0 || strcmp(input, "b") == 0) {
        // process_bid();
    } else if (strcmp(input, "show_record") == 0 || strcmp(input, "sr") == 0) {
        // process_record();
        
    } else {
        fprintf(stderr, "error: unkown_command\n");
    }
}

int main(int argc, char** argv) {

    // check if command is complete

    validate_args(argc, argv);


    // Create a socket
    fd = socket(AF_INET,SOCK_DGRAM,0);
    if (fd == -1) {
        /*error*/
        exit(1);
    }

    int tries = 0;

    // main code
    while (TRUE) {

        char *input = (char *) malloc(sizeof(char) * MAX_COMMAND_LEN);

        // read from terminal
        scanf("%s", input);

        // see which command was inputted
        process_cmd(input);
    }

    return 0;
}