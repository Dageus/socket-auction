#include "../../constants.h"
#include "open.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int process_open(char* input, char* response){
    char* token;
    int i = 0;

    token = strtok(input, " ");

    while (token != NULL) {
        token = strtok(NULL, " ");
        if (i == 0) {
            strcpy(response, OPEN_CMD);
        } else if (i == 1 && strlen(token) <= MAX_NAME) {
            strcat(response, " ");
            strcat(response, token);
        } else if (i == 2 && strlen(token) <= PASSWORD_LEN) {
            strcat(response, " ");
            strcat(response, token);
        } else if (i == 3 && strlen(token) <= MAX_START_ORDER){
            strcat(response, " ");
            strcat(response, token);
        } else if (i == 4 && strlen(token) <= MAX_DURATION_ORDER){
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