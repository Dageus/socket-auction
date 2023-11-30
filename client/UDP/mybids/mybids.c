#include "mybids.h"
#include "../../constants.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int process_my_bids(char *uid, char** response){

    *response = (char *) malloc(sizeof(char) * MYBIDS_LEN);

    if (strcmp(uid, NO_UID) == 0) {
        printf("error: please login first\n");
        return -1;
    }
        
    int return_code = sprintf(*response, "%s %s\n", MYBIDS_COMMAND, uid);

    if (return_code < 0) {
        return -1;
    }

    return 0;
}
