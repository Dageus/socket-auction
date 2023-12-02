#include "myauctions.h"
#include "../../constants.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int process_myauctions(client* user, char** response){

    *response = (char *) malloc(sizeof(char) * MYAUCTIONS_LEN);

    if (strcmp(user->uid, NO_UID) == 0) {
        printf("error: please login first\n");
        return -1;
    }
        
    int return_code = sprintf(*response, "%s %s\n", MYAUCTIONS_COMMAND, user->uid);

    if (return_code < 0) {
        return -1;
    }

    return 0;
}