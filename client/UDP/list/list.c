#include "list.h"
#include <stdio.h>

int process_list(char* response){

    int return_code = sprintf(response, "%s\n", LIST_COMMAND);

    if (return_code < 0) {
        fprintf(stderr, "error: list\n");
        return -1;
    }

    return 0;
}