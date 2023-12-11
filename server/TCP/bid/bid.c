#include "bid.h"
#include "../../constants.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "../TCP.h"

int process_bid(char* input, TCP_response** response){

    char* uid = strtok(input, " ");
    char* pwd = strtok(NULL, " ");
    char* aid = strtok(NULL, " ");
    char* amount = strtok(NULL, " ");

    if (uid == NULL || pwd == NULL || aid == NULL || amount == NULL)
        return 0;

    if (strlen(uid) != UID_LEN || strlen(pwd) != PWD_LEN || strlen(aid) != AID_LEN || strlen(amount) > 8)
        /* wrong format */
        return 0;

    char* bid_dir = (char*) malloc((strlen(AUCTIONS_DIR) + strlen(aid) + strlen(BIDS) + 3) * sizeof(char));

    sprintf(bid_dir, "%s/%s/%s", AUCTIONS_DIR, aid, BIDS);

    // find directory
    struct stat st;

    if (stat(bid_dir, &st) == 0) {
        // directory exists
        // check if file exists
        char* bid_file = (char*) malloc((strlen(bid_dir) + strlen(amount) + strlen(TXT_SUFFIX) + 3) * sizeof(char));
        sprintf(bid_file, "%s/%s%s", bid_dir, amount, TXT_SUFFIX);

        // create file
        FILE* fp = fopen(bid_file, "w");
        if (fp == NULL) {
            printf("Error creating bid file\n");
            free(bid_dir);
            free(bid_file);
            return -1;
        }

        // close file
        fclose(fp);

        free(bid_dir);
        free(bid_file);
        return 1;
    } else {
        // directory doesn't exist
        free(bid_dir);
        return -1;
    }

    *response = (TCP_response*) malloc(sizeof(TCP_response));

    strcpy((*response)->msg, "ERR");

    printf("%s\n", (*response)->msg);
}