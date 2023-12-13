#include "mybids.h"
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

int get_bidded_list(char* uid, auction_list *list) {
    struct dirent **filelist;
    int n_entries ,n_bids, len;

    char dirname[strlen(USERS_DIR) + UID_LEN + strlen(BIDDED) + 4];
    sprintf(dirname, "%s/%s/%s/", USERS_DIR, uid, BIDDED);

    n_entries = scandir(dirname, &filelist, 0, alphasort);

    printf("n_entries: %d\n", n_entries);

    if (n_entries <= 0) // Could test for −1 since n_entries count always with . and ..
        return 0;

    n_bids = 0;

    while (n_entries--) {
        len = strlen(filelist[n_entries]->d_name);
        if (len == AUCTION_FILE_LEN) { // Discard '.' , '..' and invalid filenames by size 
            char aid[4];
            strncpy(aid, filelist[n_entries]->d_name, AID_LEN);

            printf("aid: %s\n", aid);

            load_auction(aid, &list[n_bids]);
            ++n_bids;

            printf("%s\n", list[n_bids].auction_code);
            printf("n_bids: %d\n", n_bids);
        }
            
        free(filelist[n_entries]);

        if (n_bids == 50)
            break;
    }

    free(filelist);

    return n_bids;
}

int process_mybids(char* input, char** response){

    char* uid = strtok(input, " ");

    if (uid == NULL) {
        *response = (char*) malloc((strlen(MYB_CMD) + 2) * sizeof(char));
        sprintf(*response, "%s ERR", MYB_CMD);
        return 0;
    }

    char user_dir[strlen(USERS_DIR) + strlen(uid) + 2];

    sprintf(user_dir, "%s/%s", USERS_DIR, uid);

    struct stat st;

    if (stat(user_dir, &st) == 0) {
        // user exists

        // check if user is logged in

        char login_file[strlen(user_dir) + UID_LEN + strlen(LOGIN_SUFFIX) + 2];

        sprintf(login_file, "%s/%s%s", user_dir, uid,  LOGIN_SUFFIX);

        if (stat(login_file, &st) == 0) {
            // user is logged in

            printf("User %s is logged in, checking bids...\n", uid);

            auction_list list[50];

            int n_bids = get_bidded_list(uid, list);

            if (n_bids == 0) {
                printf("User %s has no bids\n", uid);
                *response = (char*) malloc((strlen(MYB_CMD) + 2) * sizeof(char));
                sprintf(*response, "%s NOK", MYB_CMD);
                return 0;
            }

            *response = (char*) malloc((strlen(MYB_CMD) + strlen(OK_STATUS) + n_bids * 6) * sizeof(char));

            sprintf(*response, "%s %s ", MYB_CMD, OK_STATUS);

            for (int i = 0; i < n_bids; i++) {
                strcat(*response, list[i].auction_code);
                strcat(*response, " ");
                strcat(*response, list[i].active);
                strcat(*response, " ");
            }

            (*response)[strlen(*response) - 1] = '\n';

        } else {
            // user is not logged in
            *response = (char*) malloc((strlen(MYB_CMD) + 3 + 2) * sizeof(char));
            sprintf(*response, "%s NLG\n", MYB_CMD);
            return 0;
        }

    } else {
        // user does not exist
        *response = (char*) malloc((strlen(MYB_CMD) + 3 + 2) * sizeof(char));
        sprintf(*response, "%s ERR\n", MYB_CMD);
        return 0;
    }
}