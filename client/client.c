#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <bits/getopt_core.h>
#include "constants.h"
#include "UDP/commands_udp.h"
#include "TCP/commands_tcp.h"
#include "UDP/UDP.h"
#include "TCP/TCP.h"



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

int tcp_fd;


// initialize default values in case of incomplete command

client* user;
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

void check_UDP_cmd(char* input, char* cmd) {
    
    char* response = NULL;

    if (strcmp(cmd, "login") == 0) {
        if (process_login(input, &user, &response) == -1)
            printf("error: login\n");
        else
            // login was successful, change uid and pwd
            printf("uid: %s\npwd: %s\n", user->uid, user->pwd);
    } else if (strcmp(cmd, "logout") == 0) {
        if (process_logout(&user, &response) == -1)
            printf("error: logout\n");
    } else if (strcmp(cmd, "unregister") == 0) {
        if (process_unregister(&user, &response) == -1)
            printf("error: unregister\n");
    } else if (strcmp(cmd, "exit") == 0) {
        if (process_exit(&client) == -1)
            printf("error: please log out first\n");
        else
            // change uid and pwd to null
            uid = "000000";
    } else if (strcmp(cmd, "myauctions") == 0 || strcmp(cmd, "ma") == 0) {
        if (process_myauctions(uid, &response) == -1)
            printf("error: auctions\n");
    } else if (strcmp(cmd, "mybids") == 0 || strcmp(cmd, "mb") == 0) {
        if (process_my_bids(uid, &response) == -1)
            printf("error: bids\n");
    } else if (strcmp(cmd, "list") == 0 || strcmp(cmd, "l") == 0) {
        if (process_list(&response) == -1)
            printf("error: list\n");
    } else if (strcmp(cmd, "show_record") == 0 || strcmp(cmd, "sr") == 0){
        if (process_show_record(input, &response) == -1)
            printf("error: show_record\n"); 
    }

    printf("UDP response: %s", response);

    send_UDP(response);

    if (response != NULL)
        free(response);
}

void check_TCP_cmd(char* input, char* cmd) {

    char* response = NULL;

    if (strcmp(cmd, "bid") == 0 || strcmp(cmd, "b") == 0) {
        if (process_bid(input, &user, &response) == -1)
            printf("error: bid\n");
    } else if (strcmp(cmd, "show_asset") == 0 || strcmp(cmd, "sa") == 0) {
        if (process_show_asset(input, &response) == -1)
            printf("error: asset\n");
    } else if (strcmp(cmd, "open") == 0) {
        if (process_open(input, &response) == -1)
            printf("error: open\n");
    } else if (strcmp(cmd, "close") == 0) {
        if (process_close(input, &user, &response) == -1)
            printf("error: close\n");
    }

    printf("TCP response: %s\n", response);

    // send_TCP_cmd(response);

    if (response != NULL)
        free(response);
}

void process_cmd(char* input){

    char *input_copy, *cmd;

    input_copy = (char *) malloc(sizeof(char) * MAX_COMMAND_LEN);

    strncpy(input_copy, input, strlen(input) - 1);

    cmd = strtok(input, " ");

    if (cmd[strlen(cmd) - 1] == '\n')
        cmd[strlen(cmd) - 1] = '\0';

    if (UDP_cmd(cmd)){
        check_UDP_cmd(input_copy, cmd);
    }
    else if (TCP_cmd(cmd))
        check_TCP_cmd(input_copy, cmd);
    else
        fprintf(stderr, "error: unkown_command\n");

    free(input_copy);
    free(cmd);
}


int main(int argc, char** argv) {

    // check if command is complete

    validate_args(argc, argv);

    // initialize client, uid and pwd
    user = (client*) malloc(sizeof(client));
    user->uid = (char *) malloc(sizeof(char) * UID_LENGTH);
    strcpy(user->uid, NO_UID);

    user->pwd = (char *) malloc(sizeof(char) * PASSWORD_LEN);
    strcpy(user->pwd, NO_PWD);

    // int tries = 0;

    // main code
    while (TRUE) {
        char *input = (char *) malloc(sizeof(char) * MAX_COMMAND_LEN);

        // read from terminal
        fgets(input, MAX_COMMAND_LEN, stdin);

        // see which command was inputted
        process_cmd(input);
    }

    return 0;
}