#include "../../constants.h"
#include "open.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int process_open(char* input, client* user, char** response, int* code){
    
    if (user->uid == NULL || user->pwd == NULL) {
        fprintf(stderr, "error: user not logged in\n");
        return -1;
    }

    *response = (char *) malloc(sizeof(char) * OPEN_LEN);
    
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

    // ! é preciso saber como saacar o Fsize e onde começar a meter o Fdata

    int return_code = sprintf(*response, "%s %s %s %s %s %s %s ", 
            cmd, user->uid, user->pwd, name, start, duration, fname);

    if (return_code < 0) {
        fprintf(stderr, "error: open\n");
        return -1;
    }

    return 0;
}