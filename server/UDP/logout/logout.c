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
#include "UDP/UDP.h"

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

    char* pwd_read = (char*) malloc(sizeof(char) * (PWD_LEN + 1));
    fgets(pwd_read, sizeof(pwd_read), fp);

    if (strcmp(pwd_read, pwd) == 0)
        return 0;
    else
        return -1;
}

int process_user_logout(char* input, UDP_response** response){
    char* uid = strtok(input, " ");
    char *pwd = strtok(NULL, " ");

    if (strlen(uid) != UID_LEN || strlen(pwd) != PWD_LEN)
        return 0;

    // find directory and erase it

    char* login_dir = (char*) malloc((strlen(USERS_DIR) + strlen(LOGIN_SUFFIX) + 2*strlen(uid) + 3) * sizeof(char));

    sprintf(login_dir, "%s/%s/%s%s", USERS_DIR, uid, uid, LOGIN_SUFFIX);

    int return_code = unlink(login_dir);

    if (return_code == -1) {
        fprintf(stderr, "Error deleting login for user: %s\n", uid);
        free(login_dir);
        return -1;
    }

    free(login_dir);
    return 0;
}