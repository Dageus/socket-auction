#include "logout.h"
#include "../../constants.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int process_logout(char *uid, char* pwd, char** response){

    printf("logout command\n");

    if (strcmp(uid, NO_UID) == 0) {
        printf("error: please login first\n");
        return -1;
    }

    *response = (char *) malloc(sizeof(char) * LOGOUT_LEN);

    int return_code = sprintf(*response, "%s %s %s\n", LOGOUT_COMMAND, uid, pwd);

    if (return_code < 0) {
        fprintf(stderr, "error: logout\n");
        return -1;
    }

    return 0;
}