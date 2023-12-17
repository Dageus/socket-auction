#include "bid.h"
#include "../TCP.h"
#include "../../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int process_bid(char *input, client* user, TCP_response** response){

    printf("Processing bid...\n");

    (*response)->msg = (char *) malloc(sizeof(char) * 50);
    (*response)->file = NULL;

    printf("input: %s\n", input);
    
    char *aid = strtok(NULL, " ");
    char *amount = strtok(NULL, " ");

    printf("amount: %s\n", amount);

    sprintf((*response)->msg, "BID 123456 teste123 002 200\n", BID_CMD/*, user->uid, user->pwd, aid, amount*/);
    // sprintf((*response)->msg, "CLS %s %s %s\n", user->uid, user->pwd, aid);

    printf("%s\n", (*response)->msg);

    return 0;

}