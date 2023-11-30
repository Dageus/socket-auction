#include "myauctions.h"
#include "../../constants.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int process_myauctions(char *uid, char** response){

    *response = (char *) malloc(sizeof(char) * MYAUCTIONS_LEN);

    if (strcmp(uid, NO_UID) == 0) {
        printf("error: please login first\n");
        return -1;
    }
        
    int return_code = sprintf(*response, "%s %s\n", MYAUCTIONS_COMMAND, uid);

    if (return_code < 0) {
        return -1;
    }

    return 0;
}