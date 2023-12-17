#include "open.h"
#include "../TCP.h"
#include "../../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int process_open(char* input, client* user, TCP_response** response){
    
    if (strcmp(user->uid, NO_UID) == 0) {
        fprintf(stderr, "error: user not logged in\n");
        return -1;
    }

    (*response)->msg = (char *) malloc(sizeof(char) * OPEN_LEN);
    (*response)->file = (file *) malloc(sizeof(file));
    (*response)->file->code = FILE_TO_BE_SENT;
    (*response)->file->filename = (char *) malloc(sizeof(char) * FILENAME_MAX);
    

    char *cmd = strtok(input, " ");
    char *name = strtok(NULL, " ");
    char *fname = strtok(NULL, " ");
    char *start = strtok(NULL, " ");
    char *duration = strtok(NULL, " ");
    
    if (cmd == NULL || name == NULL || fname == NULL || start == NULL || duration == NULL) {
        fprintf(stderr, "error: open\n");
        return -1;
    }

    strcpy((*response)->file->filename, fname);

    int return_code = sprintf((*response)->msg, "OPA %s %s %s %s %s %s ", 
            user->uid, user->pwd, name, start, duration, fname);

    if (return_code < 0) {
        fprintf(stderr, "error: open\n");
        return -1;
    }
    
    return 0;
}