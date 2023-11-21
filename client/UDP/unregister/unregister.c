#include "unregister.h"
#include <stdio.h>


int process_unregister(int uid, char* response) {
    
    int return_code = sprintf(response, "%s %d\n", &UNREGISTER_COMMAND, uid);

    if (return_code < 0) {
        fprintf(stderr, "error: unregister\n");
        return -1;
    }

    return 0;
}