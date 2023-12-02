#include "unregister.h"
#include "../../constants.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int process_unregister(client* user, char** response) {

    if (strcmp(user->uid, NO_UID) == 0) {
        printf("error: please login first\n");
        return -1;
    }

    *response = (char *) malloc(sizeof(char) * UNREGISTER_LEN);
    
    int return_code = sprintf(*response, "%s %s %s\n", UNREGISTER_COMMAND, user->uid, user->pwd);

    if (return_code < 0) {
        fprintf(stderr, "error: unregister\n");
        return -1;
    }

    return 0;
}