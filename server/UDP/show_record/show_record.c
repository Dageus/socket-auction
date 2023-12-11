#include "show_record.h"
#include "../../constants.h"
#include <sys/stat.h>
#include <dirent.h>
#include "../UDP.h"

int load_auction_info(int aid, auction* auc){
    char* aid_dir = (char*) malloc((strlen(AUCTIONS_DIR) + AID_LEN + 2) * sizeof(char));

    sprintf(aid_dir, "%s/%03d", AUCTIONS_DIR, aid);

    struct stat st;

    if (stat(aid_dir, &st) == 0) {
        // directory exists

        char* aid_file = (char*) malloc((strlen(aid_dir) + strlen(START_PREFIX) + AID_LEN + strlen(TXT_SUFFIX) + 2) * sizeof(char));
        sprintf(aid_file, "%s/%s%03d%s", aid_dir, START_PREFIX, aid, TXT_SUFFIX);

        FILE* fp = fopen(aid_file, "r");

        if (fp == NULL) {
            free(aid_dir);
            return 0;
        }

        char* line = NULL;
        size_t len = 0;
        ssize_t read;

        // get line from file
        read = getline(&line, &len, fp);

        if (read == -1) {
            free(aid_dir);
            return 0;
        }

        // fill auction struct
        char* host_uid = strtok(line, " ");
        strcpy(auc->host_uid, host_uid);
        char* name = strtok(NULL, " ");
        strcpy(auc->name, name);
        auc->start_value = atoi(strtok(NULL, " "));
        auc->time_active = atoi(strtok(NULL, " "));
        char* start_date = strtok(NULL, " ");
        char* start_time = strtok(NULL, " ");
        sprintf(auc->start_time, "%s %s", start_date, start_time);

        fclose(fp);
        free(line);

        // check if the END_ file exists
        char* end_file = (char*) malloc((strlen(aid_dir) + strlen(END_PREFIX) + AID_LEN + strlen(TXT_SUFFIX) + 2) * sizeof(char));
        sprintf(end_file, "%s/%s%03d%s", aid_dir, END_PREFIX, aid, TXT_SUFFIX);

        if (stat(end_file, &st) == 0) {
            // file exists
            fp = fopen(end_file, "r");

            auc->active = ACTIVE;

            if (fp == NULL) {
                free(aid_dir);
                free(aid_file);
                free(end_file);
                return 0;
            }

            line = NULL;
            len = 0;
            read = getline(&line, &len, fp);

            if (read == -1) {
                free(aid_dir);
                free(aid_file);
                free(end_file);
                return 0;
            }

            char* end_date = strtok(line, " ");
            char* end_time = strtok(NULL, " ");
            sprintf(auc->end.end_time, "%s %s", end_date, end_time);
            auc->end.end_sec_time = atoi(strtok(NULL, " "));

            free(line);
            fclose(fp);
        } else {
            // file doesn't exist
            auc->active = ACTIVE;
        }

        free(aid_dir);

        return 1;
    } else {
        // directory doesn't exist
        free(aid_dir);
        return 0;
    }
}

int load_bid(char* pathname, bid_list list_item) {

    FILE *fp = fopen(pathname, "r");

    if (fp == NULL)
        return 0;

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    // get line from file
    read = getline(&line, &len, fp);

    if (read == -1)
        return 0;

    char* uid = strtok(line, " ");
    strcpy(list_item.bidder_uid, uid);
    list_item.bid_value = atoi(strtok(NULL, " "));
    char* bid_time = strtok(NULL, " ");
    strcpy(list_item.bid_time, bid_time);
    list_item.bid_sec_time = atoi(strtok(NULL, " "));

    fclose(fp);

    return 1;
}

int get_record_list(int aid , bid_list *list) {
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
            char filename[11];
            strncpy(filename, filelist[n_entries]->d_name, 10);
            sprintf(pathname, "%s/%03d/%s/%s", AUCTIONS_DIR, aid, BIDS, filename);
            if (load_bid(pathname, list[n_bids]))
                ++n_bids;
        }

        free(filelist[n_entries]);

        if (n_bids == 50)
            break;
    }

    free(filelist);

    return n_bids;
}

int process_show_record(char* input, char** response){

    char *aid_ = strtok(input, " ");

    if (aid_ == NULL)
        return 0;

    int aid = atoi(aid_);

    if (aid < 1 || aid > 999)
        return 0;

    bid_list list[50];
    auction auc;

    if (!load_auction_info(aid, &auc))
        return 0;

    int n_bids = get_record_list(aid, list);

    if (n_bids == 0)
        // error
        return 0;

    // ! criar mensagem de resposta
    


    return 1;

}