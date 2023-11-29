#include "myauctions.h"
#include <stdio.h>

int process_myauctions(char *uid, char* response){
        
    int return_code = sprintf(response, "%s %s\n", MYAUCTIONS_COMMAND, uid);

    if (return_code < 0) {
        return -1;
    }

    return 0;
}