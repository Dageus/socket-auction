#include "show_record.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../constants.h"

int process_show_record(client* user, char** response){

    *response = (char *) malloc(sizeof(char) * SHOW_RECORD_LEN);

    if (strcmp(user->uid, NO_UID) == 0) {
        printf("error: please login first\n");
        return -1;
    }
        
    int return_code = sprintf(*response, "%s %s\n", SHOW_RECORD_COMMAND, user->uid);

    if (return_code < 0) {
        return -1;
    }

    return 0;
}