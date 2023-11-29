#include "myauctions.h"
#include <stdio.h>
#include <string.h>

int process_myauctions(char *uid, char** response){

    if (strcmp(uid, "000000") == 0) {
        printf("error: please login first\n");
        return -1;
    }
        
    int return_code = sprintf(*response, "%s %s\n", MYAUCTIONS_COMMAND, uid);

    if (return_code < 0) {
        return -1;
    }

    return 0;
}