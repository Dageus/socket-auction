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

int process_list(char** response){

    printf("LIST command\n");
    
    DIR *dir;
    struct dirent *entry;
    int auction_count = 0;

    dir = opendir(AUCTIONS_DIR);
    if (dir == NULL) {
        perror("Unable to open directory");
        (*response) = (char*) malloc(sizeof(char) * (LIST_ERR_LEN + 2));
        sprintf(*response, "%s %s\n", LIST_CMD, ERR_STATUS);
        printf("%s\n", *response);
        return -1;
    }

    printf("Auctions:\n");
    
    auction_list auctions[999]; 

    while ((entry = readdir(dir)) != NULL) {

        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            auction_list new_auction;

            strncpy(new_auction.auction_code, entry->d_name, 3);

            printf("auction code: %s\n", new_auction.auction_code);
            
            // check if END_(AID).txt exists
            
            struct stat st;
            char end_file[strlen(AUCTIONS_DIR) + 2 * 3 + strlen(END_PREFIX) + strlen(TXT_SUFFIX) + 2];
            sprintf(end_file, "%s/%s/%s%s%s", AUCTIONS_DIR, new_auction.auction_code, END_PREFIX, new_auction.auction_code, TXT_SUFFIX);

            if (stat(end_file, &st) == 0)
                strcpy(new_auction.active, ACTIVE);
            else
                strcpy(new_auction.active, NOT_ACTIVE);

            auctions[auction_count] = new_auction;
            auction_count++;
        }
    }

    for (int i = 0; i < auction_count; i++) {
        printf("%s %s\n", auctions[i].auction_code, auctions[i].active);
    }

    closedir(dir);

    (*response) = (char*) malloc(sizeof(char) * (strlen(LIST_CMD) + 1 + strlen(OK_STATUS) + 1 + auction_count*6));

    sprintf(*response, "%s %s ", LIST_CMD, OK_STATUS);

    for (int i = 0; i < auction_count; i++) {
        strcat(*response, auctions[i].auction_code);
        strcat(*response, " ");
        strcat(*response, auctions[i].active);
        strcat(*response, " ");
    }

    (*response)[strlen(*response) - 1] = '\n';

    return 0;
}