#include "list.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int process_list(char** response){

    *response = (char *) malloc(sizeof(char) * LIST_LEN);

    int return_code = sprintf(*response, "%s\n", LIST_COMMAND);

    if (return_code < 0) {
        fprintf(stderr, "error: list\n");
        return -1;
    }

    return 0;
}