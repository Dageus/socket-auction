#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "login.h"
#include "../UDP.h"


#define LOGIN_CMD "LIN"
#define MAX_UID 6
#define MAX_PWD 8

int process_login(char* input, char* response) {


    char uid[MAX_UID + 1];
    char pwd[MAX_PWD + 1];
    char* token;
    int i = 0;

    while (token != NULL) {
        token = strtok(NULL, " ");
        if (i == 0) {
            strcpy(*response, LOGIN_CMD);
        } else if (i == 1 && strlen(token) == MAX_UID) {
            strcat(*response, " ");
            strcat(*response, token);
        } else if (i == 2 && strlen(token) == MAX_PWD) {
            strcat(*response, " ");
            strcat(*response, token);
        } else if (strlen(token) != MAX_UID || strlen(token) != MAX_PWD) {
            printf("Invalid UID or PWD\n");
            return -1;
        }
        i++;
    }

    sendUDP(response);

    return 0;
}
