#include "bid.h"
#include "../../constants.h"
#include "../../common/common.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include "../TCP.h"

int process_bid(int fd, char** response){

    char uid[UID_LEN + 1];
    char pwd[PWD_LEN + 1];
    char aid[AID_LEN + 1];
    char amount[MAX_BIDDING_LEN + 1];

    read_word(fd, uid, UID_LEN);
    read_word(fd, pwd, PWD_LEN);
    read_word(fd, aid, AID_LEN);
    read_word(fd, amount, MAX_BIDDING_LEN);

    printf("uid: %s\n", uid);
    printf("pwd: %s\n", pwd);
    printf("aid: %s\n", aid);
    printf("amount: %s\n", amount);

    if (strlen(uid) != UID_LEN || strlen(pwd) != PWD_LEN || strlen(aid) != AID_LEN || strlen(amount) > 8)
        /* wrong format */
        return 0;

    // struct to store directory
    struct stat st;

    // check if auction is open
    char end_file[strlen(AUCTIONS_DIR) + AID_LEN + strlen(END_PREFIX) + AID_LEN + strlen(TXT_SUFFIX) + 2];
    sprintf(end_file, "%s/%s/%s%s%s", AUCTIONS_DIR, aid, END_PREFIX, aid, TXT_SUFFIX);

    if (stat(end_file, &st) == 0) {
        // auction is closed
        (*response) = (char*) malloc(BID_NOK_LEN + 1);
        sprintf((*response), "%s NOK\n", BID_CMD);
        return -1;
    }

    // check if user is logged in
    char login_dir[strlen(USERS_DIR) + 2 * strlen(uid) + strlen(LOGIN_SUFFIX) + 3];
    sprintf(login_dir, "%s/%s/%s%s", USERS_DIR, uid, uid, LOGIN_SUFFIX);

    if (stat(login_dir, &st) != 0) {
        // user is not logged in
        (*response) = (char*) malloc(BID_NOK_LEN + 1);
        sprintf((*response), "%s NLG\n", BID_CMD);
        return -1;
    }

    // check if user started the auction
    char auction_dir[strlen(AUCTIONS_DIR) + strlen(aid) + strlen(START_PREFIX) + strlen(uid) + strlen(TXT_SUFFIX) + 3];
    sprintf(auction_dir, "%s/%s/%s%s%s%s", AUCTIONS_DIR, aid, START_PREFIX, uid, aid, TXT_SUFFIX);

    // open start file
    FILE* start_file = fopen(auction_dir, "r");
    if (start_file == NULL) {
        // user didn't start the auction
        (*response) = (char*) malloc(BID_ERR_LEN + 1);
        sprintf((*response), "%s ERR\n", BID_CMD);
        return -1;
    }

    // read start file
    char start_uid[UID_LEN + 1];

    if (fscanf(start_file, "%s", start_uid) != 1) {
        // error reading start file
        (*response) = (char*) malloc(BID_ERR_LEN + 1);
        sprintf((*response), "%s ERR\n", BID_CMD);
        return -1;
    }

    // check if user started the auction
    if (strcmp(start_uid, uid) == 0) {
        // user didn't start the auction
        (*response) = (char*) malloc(BID_ERR_LEN + 1);
        sprintf((*response), "%s ILG\n", BID_CMD);
        return -1;
    }

    // close start file
    fclose(start_file);

    char bid_dir[strlen(AUCTIONS_DIR) + strlen(aid) + strlen(BIDS) + 3];
    sprintf(bid_dir, "%s/%s/%s", AUCTIONS_DIR, aid, BIDS);

    char bid_file[strlen(bid_dir) + strlen(amount) + strlen(TXT_SUFFIX) + 3];
    sprintf(bid_file, "%s/%s%s", bid_dir, amount, TXT_SUFFIX);
    

    if (stat(bid_dir, &st) == 0) {
        // directory exists
        // check if file exists
        char bid_file[strlen(bid_dir) + strlen(amount) + strlen(TXT_SUFFIX) + 3];
        sprintf(bid_file, "%s/%s%s", bid_dir, amount, TXT_SUFFIX);

        // sort bid directory
        struct dirent** bid_file_list;
        int n_entries;

        n_entries = scandir(bid_dir, &bid_file_list, 0, alphasort);

        if (n_entries < 0) {
            // error reading directory
            (*response) = (char*) malloc(BID_ERR_LEN + 1);
            sprintf((*response), "%s ERR\n", BID_CMD);
            return -1;
        }

        // check highest bid
        char highest_bid_file[7];
        strncpy(highest_bid_file, bid_file_list[n_entries - 1]->d_name, 6);

        int highest_bid = atoi(highest_bid_file);

        printf("highest bid: %d\n", highest_bid);

        if (atoi(amount) <= highest_bid) {
            // bid is lower than highest bid
            (*response) = (char*) malloc(BID_NOK_LEN + 1);
            sprintf((*response), "%s REF\n", BID_CMD);
            return -1;
        }

        return 1;

    } else {
        // directory doesn't exist
        (*response) = (char*) malloc(BID_NOK_LEN + 1);
        sprintf((*response), "%s NOK\n", BID_CMD);
        return -1;
    }

    (*response) = (char*) malloc(BID_ACC_LEN + 1);

    sprintf((*response), "%s ACC\n", BID_CMD);
}