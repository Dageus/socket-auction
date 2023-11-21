#include "logout.h"
#include <stdio.h>

int process_logout(int uid, char* response){

    int return_code = sprintf(response, "%s %d\n", &LOGOUT_COMMAND, uid);

    if (return_code < 0) {
        fprintf(stderr, "error: logout\n");
        return -1;
    }

    return 0;
}