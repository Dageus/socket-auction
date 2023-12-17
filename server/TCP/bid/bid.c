#include "bid.h"
#include "../../constants.h"
#include "../../common/common.h"
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

void process_bid(int fd, char** response){

    char uid[UID_LEN + 1];
    char pwd[PWD_LEN + 1];
    char aid[AID_LEN + 1];
    char amount[MAX_BIDDING_LEN + 1];

    read_word(fd, uid, UID_LEN + 1);

    if (strlen(uid) != UID_LEN) {
        /* wrong format */
        clear_socket(fd);
        (*response) = (char*) malloc(BID_NOK_LEN + 1);
        sprintf((*response), "%s NOK\n", BID_CMD);
        return;
    }

    read_word(fd, pwd, PWD_LEN + 1);
    
    read_word(fd, aid, AID_LEN + 1);
    
    read_word(fd, amount, MAX_BIDDING_LEN + 1);

    check_auction_end(atoi(aid));

    // struct to store directory
    struct stat st;

    // check if auction is open
    char end_file[strlen(AUCTIONS_DIR) + AID_LEN + strlen(END_PREFIX) + AID_LEN + strlen(TXT_SUFFIX) + 2];
    sprintf(end_file, "%s/%s/%s%s%s", AUCTIONS_DIR, aid, END_PREFIX, aid, TXT_SUFFIX);

    if (stat(end_file, &st) == 0) {
        printf("auction is closed\n");
        // auction is closed
        (*response) = (char*) malloc(BID_NOK_LEN + 1);
        sprintf((*response), "%s NOK\n", BID_CMD);
        return;
    }

    // check if user is logged in
    char login_dir[strlen(USERS_DIR) + 2 * strlen(uid) + strlen(LOGIN_SUFFIX) + 3];
    sprintf(login_dir, "%s/%s/%s%s", USERS_DIR, uid, uid, LOGIN_SUFFIX);

    if (stat(login_dir, &st) != 0) {
        // user is not logged in
        (*response) = (char*) malloc(BID_NOK_LEN + 1);
        sprintf((*response), "%s NLG\n", BID_CMD);
        return;
    }

    // check if user started the auction
    char auction_dir[strlen(AUCTIONS_DIR) + strlen(aid) + strlen(START_PREFIX) + strlen(aid) + strlen(TXT_SUFFIX) + 3];
    sprintf(auction_dir, "%s/%s/%s%s%s", AUCTIONS_DIR, aid, START_PREFIX, aid, TXT_SUFFIX);

    printf("auction_dir: %s\n", auction_dir);

    // open start file
    FILE* start_file = fopen(auction_dir, "r");
    if (start_file == NULL) {
        printf("error opening start file\n");
        // user didn't start the auction
        (*response) = (char*) malloc(BID_ERR_LEN + 1);
        sprintf((*response), "%s ERR\n", BID_CMD);
        return;
    }

    // read start file
    char start_uid[UID_LEN + 1];

    if (fscanf(start_file, "%s", start_uid) != 1) {
        printf("error reading start file\n");
        // error reading start file
        (*response) = (char*) malloc(BID_ERR_LEN + 1);
        sprintf((*response), "%s ERR\n", BID_CMD);
        return;
    }

    // check if user started the auction
    if (strcmp(start_uid, uid) == 0) {
        // user didn't start the auction
        (*response) = (char*) malloc(BID_ERR_LEN + 1);
        sprintf((*response), "%s ILG\n", BID_CMD);
        return;
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

        printf("scanning bid directory\n");

        if (n_entries < 0) {
            // error reading directory
            (*response) = (char*) malloc(BID_ERR_LEN + 1);
            sprintf((*response), "%s ERR\n", BID_CMD);
            return;
        }

        int bid_files = 0;

        int entries = n_entries;

        while (entries--) {
            if (bid_file_list[entries]->d_type == DT_REG)
                bid_files++;
        }

        if (bid_files == 0){

            printf("bid_files: %d\n", bid_files);

            char start_file[strlen(AUCTIONS_DIR) + strlen(aid) + strlen(START_PREFIX) + strlen(aid) + strlen(TXT_SUFFIX) + 3];
            sprintf(start_file, "%s/%s/%s%s%s", AUCTIONS_DIR, aid, START_PREFIX, aid, TXT_SUFFIX);
            
            FILE *sfile = fopen(start_file, "r");

            if (sfile == NULL){
                // error reading start file
                (*response) = (char*) malloc(BID_ERR_LEN + 1);
                sprintf((*response), "%s ERR\n", BID_CMD);
                return;
            }

            char file_content[50];

            if (fgets(file_content, 50, sfile) == NULL) {
                // error reading start file
                (*response) = (char*) malloc(BID_ERR_LEN + 1);
                sprintf((*response), "%s ERR\n", BID_CMD);
                return;
            }

            fclose(sfile);

            strtok(file_content, " ");
            strtok(NULL, " ");
            strtok(NULL, " ");
            int startvalue = atoi(strtok(NULL, " "));

            if (atoi(amount) < startvalue){
                // bid is lower than start value
                (*response) = (char*) malloc(BID_NOK_LEN + 1);
                sprintf((*response), "%s REF\n", BID_CMD);
                return;
            } else{

                FILE *bfile = fopen(bid_file, "w");

                if (bfile == NULL){
                    // error reading start file
                    (*response) = (char*) malloc(BID_ERR_LEN + 1);
                    sprintf((*response), "%s ERR\n", BID_CMD);
                    return;
                }

                fprintf(bfile, "%s", uid);

                fclose(bfile);

                (*response) = (char*) malloc(BID_ACC_LEN + 1);
                sprintf((*response), "%s ACC\n", BID_CMD);
                return;
            
            }

        }

        // check highest bid
        char highest_bid_file[7];

        printf("d->d_name: %s\n", bid_file_list[n_entries - 1]->d_name);
        strncpy(highest_bid_file, bid_file_list[n_entries - 1]->d_name, 6);

        int highest_bid = atoi(highest_bid_file);

        if (atoi(amount) <= highest_bid) {
            // bid is lower than highest bid
            (*response) = (char*) malloc(BID_NOK_LEN + 1);
            sprintf((*response), "%s REF\n", BID_CMD);
            while (n_entries--) {
                printf("freeing bid_file_list[%d]\n", n_entries);
                free(bid_file_list[n_entries]);
            }

            free(bid_file_list);
            return;
        }

        while (n_entries--) {
            printf("freeing bid_file_list[%d]\n", n_entries);
            free(bid_file_list[n_entries]);
        }

        free(bid_file_list);

    } else {
        // directory doesn't exist
        printf("[INFO]: Bidding directory doesn't exist\n");
        (*response) = (char*) malloc(BID_NOK_LEN + 1);
        sprintf((*response), "%s NOK\n", BID_CMD);
        return;
    }

    FILE *bfile = fopen(bid_file, "w");

    if (bfile == NULL){
        // error reading start file
        (*response) = (char*) malloc(BID_ERR_LEN + 1);
        sprintf((*response), "%s ERR\n", BID_CMD);
        return;
    }

    fprintf(bfile, "%s", uid);

    fclose(bfile);

    (*response) = (char*) malloc(BID_ACC_LEN + 1);
    sprintf((*response), "%s ACC\n", BID_CMD);
    return;
}