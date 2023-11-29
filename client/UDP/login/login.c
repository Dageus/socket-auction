#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../../constants.h"
#include "login.h"
#include "../UDP.h"


int process_login(char* input, char* response) {
    
    char* token;
    int i = 0;

    token = strtok(input, " ");

    while (token != NULL) {
        token = strtok(NULL, " ");
        if (i == 0) {
            strcpy(response, LOGIN_CMD);
        } else if (i == 1 && strlen(token) == UID_LENGTH) {
            strcat(response, " ");
            strcat(response, token);
        } else if (i == 2 && strlen(token) == PASSWORD_LEN) {
            strcat(response, " ");
            strcat(response, token);
        } else {
            printf("Invalid UID or PWD\n");
            return -1;
        }
        i++;
    }

    return 0;
}
