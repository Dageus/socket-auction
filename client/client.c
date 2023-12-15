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
    } else if (strcmp(token, "RUR") == 0){
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
        printf("Unknown response\n");
}

void check_UDP_cmd(char* input, char* cmd) {
    
    char* request = NULL;
    char* UDP_buffer = NULL;

    if (strcmp(cmd, "login") == 0) {
        if (process_login(input, &user, &request) == -1)
            printf("error: login\n");
        printf("user->uid: %s\n", user->uid);
    } else if (strcmp(cmd, "logout") == 0) {
        if (process_logout(user, &request) == -1)
            printf("error: logout\n");
    } else if (strcmp(cmd, "unregister") == 0) {
        if (process_unregister(user, &request) == -1)
            printf("error: unregister\n");
    } else if (strcmp(cmd, "exit") == 0) {
        if (process_exit(&user) == -1)
            printf("error: please log out first\n");
    } else if (strcmp(cmd, "myauctions") == 0 || strcmp(cmd, "ma") == 0) {
        if (process_myauctions(user, &request) == -1)
            printf("error: auctions\n");
    } else if (strcmp(cmd, "mybids") == 0 || strcmp(cmd, "mb") == 0) {
        if (process_my_bids(user, &request) == -1)
            printf("error: bids\n");
    } else if (strcmp(cmd, "list") == 0 || strcmp(cmd, "l") == 0) {
        if (process_list(&request) == -1)
            printf("error: list\n");
    } else if (strcmp(cmd, "show_record") == 0 || strcmp(cmd, "sr") == 0){
        if (process_show_record(user, input, &request) == -1)
            printf("error: show_record\n"); 
    }


    if (request != NULL){
        printf("UDP request: %s", request);

        send_UDP(request, &UDP_buffer, ip, port);

        free(request);

        printf("UDP response: %s\n", UDP_buffer);

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

    char *input_copy, *cmd;

    input_copy = (char *) malloc(sizeof(char) * MAX_COMMAND_LEN);

    strncpy(input_copy, input, strlen(input) - 1);

    cmd = strtok(input, " ");

    if (cmd[strlen(cmd) - 1] == '\n')
        cmd[strlen(cmd) - 1] = '\0';


    if (UDP_cmd(cmd))
        check_UDP_cmd(input_copy, cmd);
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