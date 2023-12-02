#include "mybids.h"
#include "../../constants.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int process_my_bids(client* user, char** response){

    *response = (char *) malloc(sizeof(char) * MYBIDS_LEN);

    if (strcmp(user->uid, NO_UID) == 0) {
        printf("error: please login first\n");
        return -1;
    }
        
    int return_code = sprintf(*response, "%s %s\n", MYBIDS_COMMAND, user->uid);

    if (return_code < 0) {
        return -1;
    }

    return 0;
}
