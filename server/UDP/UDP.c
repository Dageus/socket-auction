#include "UDP.h"
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <bits/getopt_core.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>

const char* UDP_commands[] = {
    "LIN",
    "LOU",
    "UNR",
    "LMA",
    "LMB",
    "LST",
    "SRC"
};

int check_password(char* user_dir, char* uid, char* pwd){

    char pwd_file[strlen(uid) + strlen(PWD_SUFFIX) + 1];
    sprintf(pwd_file, "%s%s", uid, PWD_SUFFIX);

    printf("pwd_file: %s\n", pwd_file);

    char pwd_dir[strlen(user_dir) + strlen(pwd_file) + 2];
    sprintf(pwd_dir, "%s/%s", user_dir, pwd_file);

    printf("pwd_dir: %s\n", pwd_dir);

    FILE* fp = fopen(pwd_dir, "r");

    if (fp == NULL) {
        fprintf(stderr, "Error opening password file\n");
        return -1;
    }

    char pwd_read[PWD_LEN + 1];
    fgets(pwd_read, PWD_LEN + 1, fp);

    printf("pwd_read: %s\n", pwd_read);

    if (strcmp(pwd_read, pwd) == 0){
        fclose(fp);
        return 0;
    }
    else {
        printf("password is different\n");
        fclose(fp);
        return -1;
    }
}


void load_auction(char* aid, auction_list *list_item){

    char end_file[12 + strlen(END_PREFIX) + AID_LEN + strlen(TXT_SUFFIX) + 2];
    sprintf(end_file, "%s/%s/%s%s%s", AUCTIONS_DIR, aid, END_PREFIX, aid, TXT_SUFFIX);
    struct stat st;

    strcpy((*list_item).auction_code, aid);

    if (stat(end_file, &st) == 0) {
        // file exists

        strcpy((*list_item).active, ACTIVE);
    } else {
        // directory doesn't exist
        strcpy((*list_item).active, NOT_ACTIVE);
    }
}

int UDP_cmd(char* cmd){
    int i;
    for (i = 0; i < 6; i++) {
        if (strcmp(cmd, UDP_commands[i]) == 0) {
            return 1;
        }
    }
    return -1;
}



