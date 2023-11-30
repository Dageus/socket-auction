#include "../../constants.h"
#include "open.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int process_open(char* input, char** response){
    
    *response = (char *) malloc(sizeof(char) * OPEN_LEN);
    
    char* token, *cmd, *name, *pwd, *start_order, *duration_order;
    
    int i = 0;

    cmd = (char *) malloc(sizeof(char) * strlen(OPEN_CMD) + 1);
    name = (char *) malloc(sizeof(char) * MAX_NAME);
    pwd = (char *) malloc(sizeof(char) * PASSWORD_LEN);
    start_order = (char *) malloc(sizeof(char) * MAX_START_ORDER);
    duration_order = (char *) malloc(sizeof(char) * MAX_DURATION_ORDER);
    

    token = strtok(input, " ");

    while (token != NULL) {
        token = strtok(NULL, " ");
        if (i == 0) 
            strcpy(cmd, OPEN_CMD);
        else if (i == 1 && strlen(token) <= MAX_NAME) 
            strcpy(name, token);
        else if (i == 2 && strlen(token) <= PASSWORD_LEN) 
            strcpy(pwd, token);
        else if (i == 3 && strlen(token) <= MAX_START_ORDER)
            strcpy(start_order, token);
        else if (i == 4 && strlen(token) <= MAX_DURATION_ORDER)
            strcpy(duration_order, token);
        else {
            printf("Invalid UID or PWD\n");
            return -1;
        }
        i++;
    }

    int return_code = sprintf(*response, "%s %s %s %s %s", cmd, name, pwd, start_order, duration_order);

    if (return_code < 0) {
        fprintf(stderr, "error: open\n");
        return -1;
    }

    return 0;
}