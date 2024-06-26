#include "login.h"
#include "../../constants.h"
#include "../../common/common.h"
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

int create_user_bid_dir(char* user_dir){
    char bids_dir[strlen(user_dir) + strlen(BIDDED) + 2];
    sprintf(bids_dir, "%s/%s", user_dir, BIDDED);

    if (mkdir(bids_dir, 0777) == -1) {
        printf("Error creating bids directory\n");
        return -1;
    }
}

int create_user_auction_dir(char* user_dir){
    char auctions_dir[strlen(user_dir) + strlen(HOSTED) + 2];
        sprintf(auctions_dir, "%s/%s", user_dir, HOSTED);

        if (mkdir(auctions_dir, 0777) == -1) {
            printf("Error creating auctions directory\n");
            return -1;
        }

    return 1;
}

int create_password_file(char* user_dir, char* uid, char* pwd){
    char pwd_file[strlen(uid) + strlen(PWD_SUFFIX) + 1];
    sprintf(pwd_file, "%s%s", uid, PWD_SUFFIX);

    char pwd_dir[strlen(user_dir) + strlen(pwd_file) + 2];
    sprintf(pwd_dir, "%s/%s", user_dir, pwd_file);

    FILE* fp = fopen(pwd_dir, "w");

    if (fp == NULL) {
        fprintf(stderr, "Error creating password file\n");
        return -1;
    }

    fprintf(fp, "%s\n", pwd);

    fclose(fp);

    printf("[INFO]: Password file created\n");

    return 0;
}

int create_login_file(char* user_dir, char* uid){
    char lin_file[strlen(uid) + strlen(LOGIN_SUFFIX) + 1];
    sprintf(lin_file, "%s%s", uid, LOGIN_SUFFIX);

    char lin_dir[strlen(user_dir) + strlen(lin_file) + 2];
    sprintf(lin_dir, "%s/%s", user_dir, lin_file);

    FILE* fp = fopen(lin_dir, "w");

    if (fp == NULL) {
        fprintf(stderr, "Error creating login file\n");
        return -1;
    }

    fclose(fp);

    printf("[INFO]: Login file created\n");

    return 0;
}

void process_user_login(char* input, char** response){
    char* uid = strtok(input, " ");
    char* pwd = strtok(NULL, " ");

    if (uid == NULL || pwd == NULL || strlen(uid) != UID_LEN || strlen(pwd) != PWD_LEN || check_alphanumeric(pwd) == -1 || check_digits(uid) == -1){
        (*response) = (char*) malloc(sizeof(char) * (8));
        sprintf(*response, "RLI NOK\n");
        return;
    }

    // check if user already exists in USERS directory
    char user_dir[strlen(USERS_DIR) + strlen(uid) + 2];
    sprintf(user_dir, "%s/%s", USERS_DIR, uid);

    struct stat st;

    if (stat(user_dir, &st) == 0) {
        if (S_ISDIR(st.st_mode)){
            
            // check if password file exists
            char pwd_file[strlen(uid) + strlen(PWD_SUFFIX) + 1];
            sprintf(pwd_file, "%s%s", uid, PWD_SUFFIX);

            char pwd_dir[strlen(user_dir) + strlen(pwd_file) + 2];
            sprintf(pwd_dir, "%s/%s", user_dir, pwd_file);

            if (stat(pwd_dir, &st) == 0){

                // check if password is correct
            
                if (check_password(user_dir, uid, pwd) == 0){
                    // which means we're logging in

                    //create login file

                    if (create_login_file(user_dir, uid) == -1){
                        fprintf(stderr, "Error creating login file\n");
                        *response = (char*) malloc(sizeof(char) * (3 + 1));
                        sprintf(*response, "%s\n", ERR_STATUS);
                        return;
                    }

                    *response = (char*) malloc(sizeof(char) * (OK_LEN + 1));
                    sprintf(*response, "%s %s\n", LOGIN_RESPONSE, OK_STATUS);
                    return;
                }
                else {
                    // which means password is incorrect
                    *response = (char*) malloc(sizeof(char) * (OK_LEN + 1));
                    sprintf(*response, "%s %s\n", LOGIN_RESPONSE, NOK_STATUS);
                    return;
                }
            } else {

                // create password file
                if (create_password_file(user_dir, uid, pwd) == -1){
                    fprintf(stderr, "Error creating password file\n");
                    *response = (char*) malloc(sizeof(char) * (3 + 1));
                    sprintf(*response, "%s\n", ERR_STATUS);
                    return;
                }

                // create login file
                if (create_login_file(user_dir, uid) == -1){
                    fprintf(stderr, "Error creating login file\n");
                    *response = (char*) malloc(sizeof(char) * (3 + 1));
                    sprintf(*response, "%s\n", ERR_STATUS);
                    return;
                }

                (*response) = (char*) malloc(sizeof(char) * (REG_LEN + 1));
                sprintf(*response, "%s %s\n", LOGIN_RESPONSE, REG_STATUS);
                return;
            }
        }
    } else {
        // which means we're registering a new user
        // create the user directory and all files/directories inside

        printf("Creating new user...\n");

        // create user directory
        if (mkdir(user_dir, S_IRWXU | S_IRWXG | S_IRWXO) == -1) {
            fprintf(stderr, "Error creating user directory\n");
            *response = (char*) malloc(sizeof(char) * (3 + 1));
            sprintf(*response, "%s\n", ERR_STATUS);
            return;
        }

        // create password file
        if (create_password_file(user_dir, uid, pwd) == -1){
            fprintf(stderr, "Error creating password file\n");
            *response = (char*) malloc(sizeof(char) * (3 + 1));
            sprintf(*response, "%s\n", ERR_STATUS);
            return;
        }

        // create login file
        if (create_login_file(user_dir, uid) == -1){
            fprintf(stderr, "Error creating login file\n");
            *response = (char*) malloc(sizeof(char) * (3 + 1));
            sprintf(*response, "%s\n", ERR_STATUS);
            return;
        }

        // create auctions directory
        if (create_user_auction_dir(user_dir) == -1){
            fprintf(stderr, "Error creating auctions directory\n");
            *response = (char*) malloc(sizeof(char) * (3 + 1));
            sprintf(*response, "%s\n", ERR_STATUS);
            return;
        }

        // create bids directory
        if (create_user_bid_dir(user_dir) == -1){
            fprintf(stderr, "Error creating bids directory\n");
            *response = (char*) malloc(sizeof(char) * (3 + 1));
            sprintf(*response, "%s\n", ERR_STATUS);
            return;
        }

        *response = (char*) malloc(sizeof(char) * (REG_LEN + 1));
        sprintf(*response, "%s %s\n", LOGIN_RESPONSE, REG_STATUS);
    }
}