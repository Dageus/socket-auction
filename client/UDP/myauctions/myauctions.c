#include "myauctions.h"
#include <stdio.h>

int process_myauctions(int uid, char* response){
        
    int return_code = sprintf(response, "%s %d\n", &MYAUCTIONS_COMMAND, uid);

    if (return_code < 0) {
        return -1;
    }

    return 0;
}