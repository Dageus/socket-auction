#include "bid.h"
#include "../TCP.h"
#include "../../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int process_bid(char *input, client* user, TCP_response** response){

    (*response)->msg = (char *) malloc(sizeof(char) * 50);
    (*response)->file = NULL;
    
    char *aid = strtok(NULL, " ");
    char *amount = strtok(NULL, " ");

    sprintf((*response)->msg, "%s %s %s %s %s\n", BID_CMD, user->uid, user->pwd, aid, amount);

    printf("%s\n", (*response)->msg);

    return 0;

}