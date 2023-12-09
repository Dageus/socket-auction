#include "show_record.h"
#include "../../constants.h"
#include <dirent.h>

// TODO : load_bid

int load_bid(char* pathname, bid_list *list) {
    
    // ! nao sei oqq esta é suposto fazer

}

int get_record_list(int aid , bid_list *list, auction auc) {
    struct dirent ** filelist;
    int n_entries, n_bids, len;
    char dirname[20];
    char pathname[32];

    sprintf(dirname, "%s/%03d/%s/", AUCTIONS_DIR, aid, BIDS);

    n_entries = scandir(dirname, &filelist, 0, alphasort);

    if (n_entries <= 0) // Could test for −1 since n_entries count always with . and ..
        return 0;

    n_bids = 0;

    while ( n_entries --) {
        len = strlen(filelist[n_entries]->d_name);
        if (len == 10) { // Discard ’.’ , ’..’ and invalid filenames by size
            sprintf(pathname, "%s/%03d/%s/%s", AUCTIONS_DIR, aid, BIDS, filelist[n_entries]->d_name);
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

int process_show_record(char* input){

    char *aid_ = strtok(input, " ");

    int aid = atoi(aid_);

    if (aid < 1 || aid > 999)
        return 0;

    bid_list list[50];
    auction auc;

    int n_bids = get_record_list(aid, list, auc);

    if (n_bids == 0)
        // error
        return 0;

    // * create response

    return 1;

}