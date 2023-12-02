#include "bid.h"
#include "../../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int process_bid(char *input, client* user, char** response, int *code){

    *response = (char *) malloc(sizeof(char) * BID_LEN);

    char* token, *aid, *amount;
    aid = (char *) malloc(sizeof(char) * AID_LEN);
    amount = (char *) malloc(sizeof(char) * MAX_BIDDING_LEN);
    int i = 0;
    
    token = strtok(input, " ");

    while (token != NULL) {
        token = strtok(NULL, " ");
        if (i == 1 && strlen(token) <= AID_LEN) {
            strcpy(aid, token);
        } else if (i == 2) {
            strcpy(amount, token);
        } else {
            printf("Invalid AID\n");
            return -1;
        }
        i++;
    }

    sprintf(*response, "%s %s %s %s %s\n", BID_CMD, user->uid, user->pwd, aid, amount);

    return 0;

}