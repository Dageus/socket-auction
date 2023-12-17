#include "unregister.h"
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
#include <dirent.h>
#include "../UDP.h"

void process_unregister(char* input, char** response){
    char* uid = strtok(input, " ");
    char* pwd = strtok(NULL, " ");

    if (uid == NULL || pwd == NULL || strlen(uid) != UID_LEN || strlen(pwd) != PWD_LEN || check_alphanumeric(pwd) == -1 || check_digits(uid) == -1){
        /* wrong format */
        (*response) = (char*) malloc(sizeof(char) * (UNREGISTER_NOK_LEN + 1));
        sprintf(*response, "%s %s\n", RUR_CMD, ERR_STATUS);
        return;
    }

    // find directory
    struct stat st;

    char user_dir[strlen(USERS_DIR) + strlen(uid) + 2];
    sprintf(user_dir, "%s/%s", USERS_DIR, uid);

    if (stat(user_dir, &st) == 0) {

        char login_dir[strlen(USERS_DIR) + strlen(LOGIN_SUFFIX) + 2*strlen(uid) + 3];
        char pwd_dir[strlen(USERS_DIR) + strlen(PWD_SUFFIX) + 2*strlen(uid) + 3];

        sprintf(login_dir, "%s/%s/%s%s", USERS_DIR, uid, uid, LOGIN_SUFFIX);
        sprintf(pwd_dir, "%s/%s/%s%s", USERS_DIR, uid, uid, PWD_SUFFIX);

        if (stat(login_dir, &st) == 0) {
            // user is logged in, logout

            if (unlink(pwd_dir) == -1) {
                printf("Error removing login file\n");
                (*response) = (char*) malloc(sizeof(char) * (UNREGISTER_NOK_LEN + 1));
                sprintf(*response, "%s %s\n", RUR_CMD, ERR_STATUS);
                return;
            }

            // remove login file as well
            if (unlink(login_dir) == -1) {
                printf("Error removing login file\n");
                (*response) = (char*) malloc(sizeof(char) * (UNREGISTER_NOK_LEN + 1));
                sprintf(*response, "%s %s\n", RUR_CMD, ERR_STATUS);
                return;
            }

            (*response) = (char*) malloc(sizeof(char) * (UNREGISTER_OK_LEN + 1));
            sprintf(*response, "%s %s\n", RUR_CMD, OK_STATUS);
        } else {
            // user was not logged in

            (*response) = (char*) malloc(sizeof(char) * (UNREGISTER_NOK_LEN + 1));
            sprintf(*response, "%s %s\n", RUR_CMD, NOK_STATUS);

            return;
        }
    } else {
        // user is not registered
        
        (*response) = (char*) malloc(sizeof(char) * (UNREGISTER_UNR_LEN + 1));
        sprintf(*response, "%s %s\n", RUR_CMD, UNR_CMD);
    }
}