#include "show_record.h"
#include "../../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int process_show_record(char* input, char* uid, char* pwd, char* response) {
    
    char* token;
    token = strtok(input, " ");
    free(token);

    sprintf(response, "%s %s %s", SHOW_RECORD_CMD, uid, pwd);

    return 0;
}