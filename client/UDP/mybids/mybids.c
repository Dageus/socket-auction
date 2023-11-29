#include "mybids.h"
#include <stdio.h>

int process_my_bids(char *uid, char* response){
        
    int return_code = sprintf(response, "%s %s\n", MYBIDS_COMMAND, uid);

    if (return_code < 0) {
        return -1;
    }

    return 0;
}
