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

// TODO : load_bid

int load_bid(char* pathname, bid_list *list){

    // ! nao sei oqq esta é suposto fazer

}

int get_bidded_list(char* uid, bid_list *list) {
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
            if (load_bid(pathname, list))
                ++n_bids;
        }
            
        free(filelist[n_entries]);

        if (n_bids == 50)
            break;
    }

    free(filelist);

    return n_bids;
}

int process_myauctions(char* input){
    char* uid = (char*) malloc(UID_LEN * sizeof(char));

    char* user_dir = (char*) malloc((strlen(USERS_DIR) + strlen(uid) + 2) * sizeof(char));

    sprintf(user_dir, "%s/%s", USERS_DIR, uid);

    struct stat st;

    if (stat(user_dir, &st) == -1) {
        printf("Error getting user directory\n");
        return -1;
    } else if (!S_ISDIR(st.st_mode)) {
        printf("Error: user directory is not a directory\n");
        return -1;
    }

    bid_list list[50];

    int n_bids = get_bidded_list(uid, list);

}