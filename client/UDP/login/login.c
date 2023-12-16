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


int process_login(char* input, client** user, char** response) {

    *response = (char*) malloc(sizeof(char) * LOGIN_LEN);

    if (strcmp((*user)->uid, NO_UID) != 0){
        printf("Already logged in\n");
        return -1;
    }
    
    strtok(input, " ");

    strcpy((*user)->uid, strtok(NULL, " "));
    strcpy((*user)->pwd, strtok(NULL, " "));

    int return_code = sprintf(*response, "%s %s %s\n", LOGIN_CMD, (*user)->uid, (*user)->pwd);

    if (return_code < 0)
        return -1;

    return 0;
}
