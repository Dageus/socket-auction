#include "list.h"
#include "../../constants.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "../UDP.h"

int process_list(char* input, char** response){

    char* uid = strtok(input, " ");
    char* pwd = strtok(NULL, " ");

    if (strlen(uid) != UID_LEN || strlen(pwd) != PWD_LEN) {
        printf("Invalid UID or PWD\n");
        return -1;
    }
    
    DIR *dir;
    struct dirent *entry;
    int auction_count = 0;

    dir = opendir(AUCTIONS_DIR);
    if (dir == NULL) {
        perror("Unable to open directory");
        return -1;
    }
    
    auction_list auctions[999]; 

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            auction_list new_auction;
            strncpy(new_auction.auction_code, entry->d_name, 3);
            
            // check if END_(AID).txt exists
            
            struct stat st;
            char* end_file = (char*) malloc((strlen(AUCTIONS_DIR) + 2*strlen(new_auction.auction_code) + strlen(END_PREFIX) + strlen(TXT_SUFFIX) + 2) * sizeof(char));
            sprintf(end_file, "%s/%s/%s%s%s", AUCTIONS_DIR, new_auction.auction_code, END_PREFIX, new_auction.auction_code, TXT_SUFFIX);

            if (stat(end_file, &st) == 0)
                new_auction.active = 1;
            else
                new_auction.active = 0;

            free(end_file);

            auctions[auction_count] = new_auction;
            auction_count++;
        }
    }

    for (int i = 0; i < auction_count; i++) {
        printf("%s %d\n", auctions[i].auction_code, auctions[i].active);
    }

    closedir(dir);

    // ! complete command or do something with the auctions array

    return 0;
}