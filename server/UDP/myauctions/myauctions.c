#include "myauctions.h"
#include "../../constants.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "../UDP.h"

int get_hosted_list(char* uid, auction_list *list) {
    struct dirent **filelist;
    int n_entries ,n_bids, len;
    char *dirname;
    char *pathname;

    dirname = (char*) malloc((strlen(USERS_DIR) + UID_LEN + strlen(HOSTED) + 4) * sizeof(char));
    sprintf(dirname, "%s/%s/%s/", USERS_DIR, uid, HOSTED);

    pathname = (char*) malloc((strlen(USERS_DIR) + UID_LEN + strlen(HOSTED) + AID_LEN + strlen(TXT_SUFFIX) + 4) * sizeof(char));

    n_entries = scandir(dirname, &filelist, 0, alphasort);

    if (n_entries <= 0) // Could test for −1 since n_entries count always with . and ..
        return;

    n_bids = 0;

    while (n_entries--) {
        len = strlen(filelist[n_entries]->d_name);
        if (len == AUCTION_FILE_LEN) { // Discard '.' , '..' and invalid filenames by size 
            char aid[4];
            strncpy(aid, filelist[n_entries]->d_name, AID_LEN);
            aid[3] = '\0';
            
            load_auction(aid, &list[n_bids]);
            
            ++n_bids;
        }
            
        free(filelist[n_entries]);
    }

    free(filelist);

    return n_bids;
}

void process_myauctions(char* input, char** response){
    char* uid = strtok(input, " ");

    char login_dir[strlen(USERS_DIR) + 2*strlen(uid) + strlen(LOGIN_SUFFIX) + 4];
    sprintf(login_dir, "%s/%s/%s%s", USERS_DIR, uid, uid, LOGIN_SUFFIX);

    // check if user exists

    struct stat st;

    if (stat(login_dir, &st) == -1) {
        // user is not logged in
        printf("User is not logged in\n");
        *response = (char*) malloc((strlen(MYA_CMD) + 3 + 2) * sizeof(char));
        sprintf(*response, "%s NLG\n", MYA_CMD);
    }

    auction_list list[999];

    // pass list on to the command and fill it

    int n_bids = get_hosted_list(uid, list);

    if (n_bids == 0) {
        printf("No auctions found\n");
        *response = (char*) malloc((strlen(MYA_CMD) + 2) * sizeof(char));
        sprintf(*response, "%s NOK", MYA_CMD);
        return;
    }

    *response = (char*) malloc((strlen(MYA_CMD) + strlen(OK_STATUS) + n_bids * 6) * sizeof(char));

    sprintf(*response, "%s %s ", MYA_CMD, OK_STATUS);

    for (int i = 0; i < n_bids; i++) {
        strcat(*response, list[i].auction_code);
        strcat(*response, " ");
        strcat(*response, list[i].active);
        strcat(*response, " ");
    }

    (*response)[strlen(*response) - 1] = '\n';
}