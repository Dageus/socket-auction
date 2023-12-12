#include "close.h"
#include "../TCP.h"
#include "../../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int process_close(char *input, client* user, TCP_response** response){

    (*response)->msg = (char *) malloc(sizeof(char) * CLOSE_LEN);
    (*response)->file = NULL;

    char* token, *aid;
    int i = 0;

    aid = (char*) malloc(sizeof(char) * AID_LEN);

    token = strtok(input, " ");
    while (token != NULL) {
        token = strtok(NULL, " ");
        if (i == 1 && strlen(token) <= AID_LEN) {
            strcpy(aid, token);
        } else {
            printf("Invalid AID\n");
            return -1;
        }
        i++;
    }

    sprintf((*response)->msg, "%s %s %s %s\n", CLOSE_CMD, user->uid, user->pwd, aid);

    return 0;

}