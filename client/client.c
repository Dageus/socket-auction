#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include "constants.h"
#include "UDP/commands.h"

#define TRUE 1
#define FALSE 0

#define SERVER_IP "tejo.tecnico.ulisboa.pt"
#define TEST_PORT 58011
#define DEFAULT_PORT "58090"

#define LOCAL_SERVER_IP "localhost"

struct addrinfo
{
  int ai_flags;			/* Input flags.  */
  int ai_family;		/* Protocol family for socket.  */
  int ai_socktype;		/* Socket type.  */
  int ai_protocol;		/* Protocol for socket.  */
  socklen_t ai_addrlen;		/* Length of socket address.  */
  struct sockaddr *ai_addr;	/* Socket address for socket.  */
  char *ai_canonname;		/* Canonical name for service location.  */
  struct addrinfo *ai_next;	/* Pointer to next in list.  */
};


int fd,errcode;
ssize_t n;
socklen_t addrlen;
struct addrinfo hints, *res;
struct sockaddr_in addr;
char buffer[128];


// initialize default values in case of incomplete command

int uid = -1;
char *ip = LOCAL_SERVER_IP;
char *port = DEFAULT_PORT;

void sendUDP(char* msg) {
    fd = socket(AF_INET,SOCK_DGRAM,0);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    errcode = getaddrinfo(SERVER_IP, TEST_PORT, &hints, &res);


    n = sendto(fd, msg, strlen(msg), 0, res->ai_addr, res->ai_addrlen);
       
    addrlen = sizeof(addr);
    n = recvfrom(fd, buffer, 128, 0, (struct sockaddr*)&addr, &addrlen);


    close(fd);
}

int validate_args(int argc, char** argv) {

    int opt;

    while ((opt = getopt(argc, argv, "n:p:")) != -1) {
        switch (opt) {
            case 'n':
                ip = (char *) malloc(sizeof(char) * strlen(optarg));
                ip = optarg;
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

    char* response;

    char *n_cmd = strtok(input, " ");

    if (strcmp(n_cmd, "login") == 0) {
        if (process_login(input, response) == -1){}
        response = (char*) malloc(sizeof(char) * LOGIN_LEN);
            printf("error: login\n");
    } else if (strcmp(n_cmd, "logout") == 0) {
        response = (char *) malloc(sizeof(char) * LOGOUT_LEN);
        if (process_logout(uid, response) == -1)
            printf("error: logout\n");
    } else if (strcmp(n_cmd, "unregister") == 0) {
        response = (char *) malloc(sizeof(char) * UNREGISTER_LEN);
        if (process_unregister() == -1)
            printf("error: unregister\n");
    } else if (strcmp(n_cmd, "exit") == 0) {
        if (process_exit() == -1)
            printf("error: please log out first\n");
    } else if (strcmp(n_cmd, "open") == 0) {
        // ? TCP command, dont do yet
        response = (char *) malloc(sizeof(char) * OPEN_LEN);
        if (process_open() == -1)
            printf("error: open\n");
    } else if (strcmp(n_cmd, "close") == 0) {
        if (process_close() == -1)
            printf("error: close\n");
    } else if (strcmp(n_cmd, "myauctions") == 0 || strcmp(n_cmd, "ma") == 0) {
        response = (char *) malloc(sizeof(char) * MYAUCTIONS_LEN);
        if (process_auctions(uid) == -1)
            printf("error: auctions\n");
    } else if (strcmp(n_cmd, "mybids") == 0 || strcmp(n_cmd, "mb") == 0) {
        if (process_bids() == -1)
            printf("error: bids\n");
    } else if (strcmp(n_cmd, "list") == 0 || strcmp(n_cmd, "l") == 0) {
        response = (char *) malloc(sizeof(char) * LIST_LEN);
        if (process_list(uid) == -1)
            printf("error: list\n");
    } else if (strcmp(n_cmd, "show_asset") == 0 || strcmp(n_cmd, "sa") == 0) {
        if (process_asset() == -1)
            printf("error: asset\n");
    } else if (strcmp(n_cmd, "bid") == 0 || strcmp(n_cmd, "b") == 0) {
        if (process_bid() == -1)
            printf("error: bid\n");
    } else if (strcmp(n_cmd, "show_record") == 0 || strcmp(n_cmd, "sr") == 0) {
        response = (char *) malloc(sizeof(char) * SHOW_RECORD_LEN);
        if (process_record(input) == -1)
            printf("error: record\n");
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
        fgets(input, MAX_COMMAND_LEN, stdin);

        printf("input: %s\n", input);

        // see which command was inputted
        process_cmd(input);
    }

    return 0;
}