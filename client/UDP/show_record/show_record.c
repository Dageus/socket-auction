#include "show_record.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int process_show_record(char *uid, char** response){

    *response = (char *) malloc(sizeof(char) * SHOW_RECORD_LEN);

    if (strcmp(uid, "000000") == 0) {
        printf("error: please login first\n");
        return -1;
    }
        
    int return_code = sprintf(*response, "%s %s\n", SHOW_RECORD_COMMAND, uid);

    if (return_code < 0) {
        return -1;
    }

    return 0;
}