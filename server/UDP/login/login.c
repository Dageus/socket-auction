#include "login.h"
#include "../../constants.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <bits/getopt_core.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>

int check_password(char* user_dir, char* uid, char* pwd){

    char* pwd_file = malloc(sizeof(char) * (strlen(uid) + strlen(PWD_SUFFIX) + 1));
    sprintf(pwd, "%s%s", uid, PWD_SUFFIX);

    char* pwd_dir = (char*) malloc(sizeof(char) * (strlen(user_dir) + strlen(pwd_file) + 2));
    sprintf(pwd_file, "%s/%s", user_dir, pwd);

    FILE* fp = fopen(pwd_dir, "r");

    if (fp == NULL) {
        fprintf(stderr, "Error opening password file\n");
        return -1;
    }

    char* pwd_read = (char*) malloc(sizeof(char) * (PASSWORD_LEN + 1));
    fgets(pwd_read, sizeof(pwd_read), fp);

    if (strcmp(pwd_read, pwd) == 0)
        return 0;
    else
        return -1;
}

int process_user_login(char* input){
    char* uid = strtok(input, " ");
    char* pwd = strtok(NULL, " ");

    // check if user already exists in USERS directory
    char* user_dir = (char*) malloc(strlen(USERS_DIR) + strlen(uid) + 2);
    sprintf(user_dir, "%s/%s", USERS_DIR, uid);

    struct stat st;

    if (stat(user_dir, &st) == 0) {
        if (S_ISDIR(st.st_mode))
            // check if password is correct
            if (check_password(user_dir, uid, pwd) == 0)
                // which means we're logging in
                // ! send message to client
                return 0;
            else
                // which means password is incorrect
                // ! send message to client
                return -1;
    } else {
        // which means we're registering a new user
        // create the user directory and all files/directories inside

        // create user directory
        if (mkdir(user_dir, 0777) == -1) {
            fprintf(stderr, "Error creating user directory\n");
            return -1;
        }

        // create password file
        char* pwd_file = malloc(sizeof(char) * (strlen(uid) + strlen(PWD_SUFFIX) + 1));
        sprintf(pwd_file, "%s%s", uid, PWD_SUFFIX);

        char* pwd_dir = (char*) malloc(sizeof(char) * (strlen(user_dir) + strlen(pwd_file) + 2));
        sprintf(pwd_file, "%s/%s", user_dir, pwd);

        FILE* fp = fopen(pwd_dir, "w");

        if (fp == NULL) {
            fprintf(stderr, "Error creating password file\n");
            return -1;
        }

        fprintf(fp, "%s", pwd);

        fclose(fp);


        // create auctions directory
        char* auctions_dir = (char*) malloc(sizeof(char) * (strlen(user_dir) + strlen(HOSTED) + 2));
        sprintf(auctions_dir, "%s/%s", user_dir, HOSTED);

        if (mkdir(auctions_dir, 0777) == -1) {
            fprintf(stderr, "Error creating auctions directory\n");
            return -1;
        }

        // create bids directory
        char* bids_dir = (char*) malloc(sizeof(char) * (strlen(user_dir) + strlen(BIDDED) + 2));
        sprintf(bids_dir, "%s/%s", user_dir, BIDDED);

        if (mkdir(bids_dir, 0777) == -1) {
            fprintf(stderr, "Error creating bids directory\n");
            return -1;
        }

        // ! create response

    }

    printf("uid: %s\n", uid);
    printf("pwd: %s\n", pwd);
}