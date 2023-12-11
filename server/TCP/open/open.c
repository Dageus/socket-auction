#include "open.h"
#include "../../constants.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

int CreateAUCTIONDir(int AID) {
    char AID_dirname[15];
    char BIDS_dirname[20];
    int ret;

    if (AID < 1 || AID > 999)
        return 0;
    

    sprintf(AID_dirname, "AUCTIONS/%03d", AID);
    
    ret = mkdir(AID_dirname, 0700);
    if (ret == -1)
        return 0;
    
    sprintf(BIDS_dirname, "AUCTIONS/%03d/BIDS", AID);
    
    ret = mkdir(BIDS_dirname, 0700);
    if (ret == -1) {
        rmdir(AID_dirname);
        return 0;
    }
    
    return 1;
}

int process_open_auction(char* input, int aid){ // * aid is the global counter for aid's

    char* uid = strtok(input, " ");
    printf("uid: %s\n", uid);

    if (aid >= 999)
        // * reached the limit for auctions
        return 0;

    return 0;
}