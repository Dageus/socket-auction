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


int process_login(char* input, char** response) {
    
    // extract string from the pointer to response
    char* token, *uid, *pwd;
    uid = (char *) malloc(sizeof(char) * UID_LENGTH);
    pwd = (char *) malloc(sizeof(char) * PASSWORD_LEN);
    int i = 0;

    printf("input: %s\n", input);
    token = strtok(input, " ");

    while (token != NULL) {
        if (i == 0)
            strcpy(*response, LOGIN_CMD);
        else if (i == 1 && strlen(token) == UID_LENGTH)
            strcpy(uid, token);
        else if (i == 2 && strlen(token) == PASSWORD_LEN)
            strcpy(pwd, token);
        else {
            printf("Invalid UID or PWD\n");
            return -1;
        }
        token = strtok(NULL, " ");
        i++;
    }

    int return_code = sprintf(*response, "%s %s %s\n", LOGIN_CMD, uid, pwd);

    if (return_code < 0)
        return -1;

    return 0;
}
