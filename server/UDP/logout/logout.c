#include "logout.h"
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

int process_user_logout(char* input){
    char* uid = strtok(input, " ");
    char *pwd = strtok(NULL, " ");

    if (strlen(uid) != UID_LENGTH || strlen(pwd) != PASSWORD_LEN)
        return 0;

    char* user_dir = (char*) malloc(strlen(USERS_DIR) + strlen(uid) + 2);
    sprintf(user_dir, "%s/%s", USERS_DIR, uid);

    // find directory and erase it

    struct stat st;
    char* login_dir;

    if (stat(user_dir, &st) == 0) {
        if (S_ISDIR(st.st_mode))
            // unlink the login file
            login_dir = (char*) malloc((strlen(USERS_DIR) + strlen(uid) + strlen(uid) + strlen(LOGIN_SUFFIX) + 3) * sizeof(char));
            sprintf(login_dir, "%s/%s%s", user_dir, uid, LOGIN_SUFFIX);

            if (unlink(login_dir) == -1) {
                fprintf(stderr, "Error unlinking login file\n");
                return -1;
            }
            return 0;
    } else {
        // which means user doesn't exist
        // ! send message to client
        return -1;
    }
}