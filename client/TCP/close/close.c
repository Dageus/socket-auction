#include "close.h"
#include "../TCP.h"
#include "../../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int process_close(char *input, client* user, TCP_response** response){

    (*response)->msg = (char *) malloc(sizeof(char) * CLOSE_LEN);
    (*response)->file = NULL;

    char *aid = strtok(NULL, " ");

    if (aid == NULL || strlen(aid) != AID_LEN){
        fprintf(stderr, "error: asset_id\n");
        return -1;
    }

    sprintf((*response)->msg, "%s %s %s %s\n", CLOSE_CMD, user->uid, user->pwd, aid);

    return 0;

}