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
#include "UDP/commands_udp.h"
#include "TCP/commands_tcp.h"

// struct addrinfo
// {
//   int ai_flags;			/* Input flags.  */
//   int ai_family;		/* Protocol family for socket.  */
//   int ai_socktype;		/* Socket type.  */
//   int ai_protocol;		/* Protocol for socket.  */
//   socklen_t ai_addrlen;		/* Length of socket address.  */
//   struct sockaddr *ai_addr;	/* Socket address for socket.  */
//   char *ai_canonname;		/* Canonical name for service location.  */
//   struct addrinfo *ai_next;	/* Pointer to next in list.  */
// };

int udp_fd, tcp_fd;


// initialize default values in case of incomplete command

char *uid = "000000";
char *pwd = "00000000";
char *ip = LOCAL_SERVER_IP;
char *port = DEFAULT_PORT;

void validate_args(int argc, char** argv) {

    int opt;

    while ((opt = getopt(argc, argv, "n:p:")) != -1) {
        switch (opt) {
            case 'n':
                ip = (char *) malloc(sizeof(char) * strlen(optarg));
                ip = optarg;
                printf("ip: %s\n", ip);
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

    if (argc == 1)
        printf("ip: %s\n", ip);
}

void process_cmd(char* input){

    char* response;

    char *n_cmd = strtok(input, " ");

    if (strcmp(n_cmd, "login") == 0) {
        response = (char*) malloc(sizeof(char) * LOGIN_LEN);
        if (process_login(input, &response) == -1)
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
        if (process_exit(uid) == -1)
            printf("error: please log out first\n");
    } else if (strcmp(n_cmd, "open") == 0) {
        response = (char *) malloc(sizeof(char) * OPEN_LEN);
        if (process_open() == -1)
            printf("error: open\n");
    } else if (strcmp(n_cmd, "close") == 0) {
        response = (char *) malloc(sizeof(char) * CLOSE_LEN);
        if (process_close(input, uid, pwd, response) == -1)
            printf("error: close\n");
    } else if (strcmp(n_cmd, "myauctions") == 0 || strcmp(n_cmd, "ma") == 0) {
        response = (char *) malloc(sizeof(char) * MYAUCTIONS_LEN);
        if (process_myauctions(uid, response) == -1)
            printf("error: auctions\n");
    } else if (strcmp(n_cmd, "mybids") == 0 || strcmp(n_cmd, "mb") == 0) {
        response = (char *) malloc(sizeof(char) * MYBIDS_LEN);
        if (process_my_bids(uid, response) == -1)
            printf("error: bids\n");
    } else if (strcmp(n_cmd, "list") == 0 || strcmp(n_cmd, "l") == 0) {
        response = (char *) malloc(sizeof(char) * LIST_LEN);
        if (process_list(uid) == -1)
            printf("error: list\n");
    } else if (strcmp(n_cmd, "show_asset") == 0 || strcmp(n_cmd, "sa") == 0) {
        response = (char *) malloc(sizeof(char) * SHOW_ASSET_LEN);
        if (process_show_asset(input, uid, pwd, response) == -1)
            printf("error: asset\n");
    } else if (strcmp(n_cmd, "bid") == 0 || strcmp(n_cmd, "b") == 0) {
        response = (char *) malloc(sizeof(char) * BID_LEN);
        if (process_bid(input, uid, pwd, response) == -1)
            printf("error: bid\n");
    } else if (strcmp(n_cmd, "show_record") == 0 || strcmp(n_cmd, "sr") == 0) {
        response = (char *) malloc(sizeof(char) * SHOW_RECORD_LEN);
        if (process_show_record(input) == -1)
            printf("error: record\n");
    } else {
        fprintf(stderr, "error: unkown_command\n");
    }

    printf("response: %s\n", response);
}


int main(int argc, char** argv) {

    // check if command is complete

    validate_args(argc, argv);

    // int tries = 0;

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