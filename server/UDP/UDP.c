#include "UDP.h"
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <bits/getopt_core.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>

const char* UDP_commands[] = {
    "LIN",
    "LOU",
    "UNR",
    "LMA",
    "LMB",
    "LST",
    "SRC"
};


void load_auction(char* aid, auction_list* list_item){

    printf("[INSIDE FUNCTION] auction code: %s\n", aid);

    char end_file[12 + strlen(END_PREFIX) + AID_LEN + strlen(TXT_SUFFIX) + 2];
    sprintf(end_file, "%s/%s/%s%s%s", AUCTIONS_DIR, aid, END_PREFIX, aid, TXT_SUFFIX);
    struct stat st;

    printf("[INSIDE FUNCTION] end_file: %s\n", end_file);

    strcpy(list_item->auction_code, aid);
    printf("[INSIDE FUNCTION] auction code: %s\n", list_item->auction_code);

    if (stat(end_file, &st) == 0) {
        // file exists

        strcpy(list_item->active, NOT_ACTIVE);
    } else {
        // directory doesn't exist, so the auction is active
        strcpy(list_item->active, ACTIVE);
    }
}

int UDP_cmd(char* cmd){
    int i;
    for (i = 0; i < 6; i++) {
        if (strcmp(cmd, UDP_commands[i]) == 0) {
            return 1;
        }
    }
    return -1;
}



