#include "open.h"
#include "../TCP.h"
#include "../../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int process_open(char* input, client* user, TCP_response** response){
    
    if (user->uid == NULL || user->pwd == NULL) {
        fprintf(stderr, "error: user not logged in\n");
        return -1;
    }

    (*response)->msg = (char *) malloc(sizeof(char) * OPEN_LEN);
    (*response)->code = FILE_SENT;
    (*response)->filename = (char *) malloc(sizeof(char) * FILENAME_MAX);
    
    char* token, *cmd, *name, *fname, *start, *duration;
    
    int i = 0;

    cmd = (char *) malloc(sizeof(char) * strlen(OPEN_CMD) + 1);
    name = (char *) malloc(sizeof(char) * MAX_NAME);
    fname = (char *) malloc(sizeof(char) * PASSWORD_LEN);
    start = (char *) malloc(sizeof(char) * MAX_START_ORDER);
    duration = (char *) malloc(sizeof(char) * MAX_DURATION_ORDER);
    

    token = strtok(input, " ");

    while (token != NULL) {
        token = strtok(NULL, " ");
        if (i == 0) 
            strcpy(cmd, OPEN_CMD);
        else if (i == 1 && strlen(token) <= MAX_NAME) 
            strcpy(name, token);
        else if (i == 2 && strlen(token) <= FILENAME_MAX) 
            strcpy(fname, token);
        else if (i == 3 && strlen(token) <= MAX_START_ORDER)
            strcpy(start, token);
        else if (i == 4 && strlen(token) <= MAX_DURATION_ORDER)
            strcpy(duration, token);
        else {
            printf("Invalid UID or fname\n");
            return -1;
        }
        i++;
    }

    strcpy((*response)->filename, fname);

    int return_code = sprintf((*response)->msg, "%s %s %s %s %s %s %s ", 
            cmd, user->uid, user->pwd, name, start, duration, fname);

    if (return_code < 0) {
        fprintf(stderr, "error: open\n");
        return -1;
    }

    return 0;
}