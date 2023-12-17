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
#include "TCP/tcp_commands.h"
#include "UDP/UDP.h"
#include "TCP/TCP.h"

client* user;
char *ip = "localhost";
char *port = "58000";

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
        printf("ip: %s\nport: %s\n", ip, port);
}

void print_UDPresponse(char response[TRANSMISSION_RATE])  {   

    char* token; 

    char response_copy[TRANSMISSION_RATE];
    strcpy(response_copy, response);

    token = strtok(response_copy, " ");

    if (strcmp(token, "RLI") == 0){
        token = strtok(NULL, " ");
        token[strlen(token) - 1] = '\0'; // remove \n from token
        if (strcmp(token, "OK") == 0)
            printf("Login successful\n");
        else if (strcmp(token, "NOK") == 0){
            strcpy(user->uid, NO_UID);
            strcpy(user->pwd, NO_PWD);
            printf("Login unsuccessful\n");
        }
        else if (strcmp(token, "REG") == 0)
            printf("Client registered\n");
    } else if (strcmp(token, "RLO") == 0){
        token = strtok(NULL, " ");
        token[strlen(token) - 1] = '\0'; // remove \n from token
        if (strcmp(token, "OK") == 0){
            strcpy(user->uid, NO_UID);
            strcpy(user->pwd, NO_PWD);
            printf("Logout successful\n");
        }
        else if (strcmp(token, "NOK") == 0)
            printf("Logout unsuccessful\n");
    } else if (strcmp(token, "RUR") == 0) {
        token = strtok(NULL, " ");
        token[strlen(token) - 1] = '\0'; // remove \n from token
        if(strcmp(token, "OK") == 0){
            strcpy(user->uid, NO_UID);
            strcpy(user->pwd, NO_PWD);
            printf("Unregister successful\n");
        }
        else if(strcmp(token, "NOK") == 0)
            printf("Unregister unsuccessful\n");
    } else if (strcmp(token, "RMA") == 0)
        write(1, response, TRANSMISSION_RATE);
    else if (strcmp(token, "RMB") == 0)
        write(1, response, TRANSMISSION_RATE);
    else if (strcmp(token, "RLS") == 0){
        printf("List command\n");
        write(1, response, TRANSMISSION_RATE);
    }
    else if (strcmp(token, "RRC") == 0){
        token = strtok(NULL, " ");
        if(strcmp(token, "NOK") == 0){
            printf("Auction does not exist\n");
        }
        else if(strcmp(token, "OK") == 0){
            write(1, response, TRANSMISSION_RATE);
        }    
    }  
    else 
        printf("There was an error server side\n");
}

void check_UDP_cmd(char* input, char* cmd) {
    
    char* request = NULL;
    char* UDP_buffer = NULL;

    if (strcmp(cmd, "login") == 0) {
        if (process_login(input, &user, &request) == -1)
            fprintf(stderr, "[ERROR]: login\n");
    } else if (strcmp(cmd, "logout") == 0) {
        if (process_logout(user, &request) == -1)
            fprintf(stderr, "[ERROR]: logout\n");
    } else if (strcmp(cmd, "unregister") == 0) {
        if (process_unregister(user, &request) == -1)
            fprintf(stderr, "[ERROR]: unregister\n");
    } else if (strcmp(cmd, "exit") == 0) {
        if (process_exit(&user) == -1)
            fprintf(stderr, "[ERROR]: please log out first\n");
        else
            printf("exiting...\n");
    } else if (strcmp(cmd, "myauctions") == 0 || strcmp(cmd, "ma") == 0) {
        if (process_myauctions(user, &request) == -1)
            fprintf(stderr, "[ERROR]: auctions\n");
    } else if (strcmp(cmd, "mybids") == 0 || strcmp(cmd, "mb") == 0) {
        if (process_my_bids(user, &request) == -1)
            fprintf(stderr, "[ERROR]: bids\n");
    } else if (strcmp(cmd, "list") == 0 || strcmp(cmd, "l") == 0) {
        if (process_list(&request) == -1)
            fprintf(stderr, "[ERROR]: list\n");
    } else if (strcmp(cmd, "show_record") == 0 || strcmp(cmd, "sr") == 0){
        if (process_show_record(user, input, &request) == -1)
            fprintf(stderr, "[ERROR]: show_record\n"); 
    }

    if (request != NULL){
        if (send_UDP(request, &UDP_buffer, ip, port) == -1){
            printf("error: server did not respond\n");

            if (strcmp(cmd, "login") == 0) {
                printf("[INFO]: reverse login changes\n");
                strcpy(user->uid, NO_UID);
                strcpy(user->pwd, NO_PWD);
            }
            
            free(request);
            
            return;
        }

        print_UDPresponse(UDP_buffer);    
    }
}

void check_TCP_cmd(char* input, char* cmd) {

    TCP_response* request;
    request = (TCP_response *) malloc(sizeof(TCP_response));

    if (strcmp(cmd, "bid") == 0 || strcmp(cmd, "b") == 0) {
        if (process_bid(input, user, &request) == -1)
            printf("error: bid\n");
    } else if (strcmp(cmd, "show_asset") == 0 || strcmp(cmd, "sa") == 0) {
        if (process_show_asset(input, &request) == -1)
            printf("error: asset\n");
    } else if (strcmp(cmd, "open") == 0) {
        if (process_open(input, user, &request) == -1)
            printf("error: open\n");
    } else if (strcmp(cmd, "close") == 0) {
        if (process_close(input, user, &request) == -1)
            printf("error: close\n");
    }

    if (request->msg != NULL){
        send_TCP(request, ip, port);
        free(request->msg);
    } 

    free(request);
}

void process_cmd(char* input){

    char *cmd;

    char input_copy[MAX_COMMAND_LEN];

    strcpy(input_copy, input);

    cmd = strtok(input, " ");


    if (cmd[strlen(cmd) - 1] == '\n')
        cmd[strlen(cmd) - 1] = '\0';

    if (UDP_cmd(cmd))
        check_UDP_cmd(input_copy, cmd);
    else if (TCP_cmd(cmd))
        check_TCP_cmd(input_copy, cmd);
    else
        fprintf(stderr, "[ERROR]: unkown_command\n");
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

        char input[MAX_COMMAND_LEN];

        memset(input, '\0', MAX_COMMAND_LEN - 1);

        if (fgets(input, MAX_COMMAND_LEN, stdin) == NULL) { 
            // No valid command configuration can be longer than 128 characters so this shouldn't be an issue.
            ferror(stdin);
            exit(-1);
        }
        if (input[0] == '\n') { // If the user only pressed enter.
            continue;
        }

        int n_char = strcspn(input, "\n");

        if (input[n_char] == '\n') {
            input[n_char] = '\0';
        }

        // see which command was inputed
        process_cmd(input);
    }

    return 0;
}