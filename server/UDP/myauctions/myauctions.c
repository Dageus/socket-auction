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

// TODO : load_auction

int load_auction(char* pathname, auction_list *auction) {
    
    // ! nao sei oqq esta é suposto fazer
}

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
        return 0;

    n_bids = 0;

    while (n_entries--) {
        len = strlen(filelist[n_entries]->d_name);
        if (len == AUCTION_FILE_LEN) { // Discard '.' , '..' and invalid filenames by size 
            sprintf(pathname, "%s/%s/%s/%s", USERS_DIR, uid, HOSTED, filelist[n_entries]->d_name);
            if (load_auction(pathname, list))
                ++n_bids;
        }
            
        free(filelist[n_entries]);
    }

    free(filelist);

    return n_bids;
}

int process_myauctions(char* input){
    char* uid = (char*) malloc(UID_LEN * sizeof(char));

    char* user_dir = (char*) malloc((strlen(USERS_DIR) + 2*strlen(uid) + strlen(LOGIN_SUFFIX) + 4) * sizeof(char));

    sprintf(user_dir, "%s/%s/%s%s", USERS_DIR, uid, uid, LOGIN_SUFFIX);

    // check if user exists

    struct stat st;

    if (stat(user_dir, &st) == -1) {
        printf("Error getting user directory\n");
        return -1;
    } else {
        printf("Error: could not open user directory\n");
        return -1;
    }

    // process command itself

    auction_list list[999];

    // pass list on to the command and fill it

    int n_bids = get_hosted_list(uid, &list);

    if (n_bids == 0) {
        printf("No auctions found\n");
        return -1;
    }

    char** list = (char**) malloc(n_bids * sizeof(char*));
}