#include "mybids.h"
#include <stdio.h>

int process_mybids(int uid, char* response){
        
    int return_code = sprintf(response, "%s %d\n", &MYBIDS_COMMAND, uid);

    if (return_code < 0) {
        return -1;
    }

    return 0;
}
