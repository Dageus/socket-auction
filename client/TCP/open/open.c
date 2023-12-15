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
    (*response)->file = (file *) malloc(sizeof(file));
    (*response)->file->code = FILE_TO_BE_SENT;
    (*response)->file->filename = (char *) malloc(sizeof(char) * FILENAME_MAX);
    
    char* token, *cmd, *name, *fname, *start, *duration;
    
    int i = 0;

    token = strtok(input, " ");
    cmd = strtok(NULL, " ");
    name = strtok(NULL, " ");
    fname = strtok(NULL, " ");
    start = strtok(NULL, " ");
    duration = strtok(NULL, " ");
    
    if (token == NULL || cmd == NULL || name == NULL || fname == NULL || start == NULL || duration == NULL) {
        fprintf(stderr, "error: open\n");
        return -1;
    }

    strcpy((*response)->file->filename, fname);

    int return_code = sprintf((*response)->msg, "%s %s %s %s %s %s %s ", 
            cmd, user->uid, user->pwd, name, start, duration, fname);

    if (return_code < 0) {
        fprintf(stderr, "error: open\n");
        return -1;
    }

    return 0;
}