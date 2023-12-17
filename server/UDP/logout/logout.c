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
#include "../UDP.h"

void process_user_logout(char* input, char** response){
    char* uid = strtok(input, " ");
    char *pwd = strtok(NULL, " ");

    if (uid == NULL || pwd == NULL || strlen(uid) != UID_LEN || strlen(pwd) != PWD_LEN){
        (*response) = (char*) malloc(sizeof(char) * (LOGOUT_UNR_LEN + 1));
        sprintf(*response, "%s %s\n", LOGOUT_CMD, UNR_CMD);
    }

    // find directory and erase it

    char login_dir[strlen(USERS_DIR) + strlen(LOGIN_SUFFIX) + 2 * strlen(uid) + 3];

    sprintf(login_dir, "%s/%s/%s%s", USERS_DIR, uid, uid, LOGIN_SUFFIX);

    // check if file exists

    struct stat st;

    if (stat(login_dir, &st) == 0) {

        // user is logged in, logout

        int return_code = unlink(login_dir);

        if (return_code == -1) {
            fprintf(stderr, "Error deleting login for user: %s\n", uid);
            *response = (char*) malloc(sizeof(char) * (3 + 1));
            sprintf(*response, "RLO %s\n", ERR_STATUS);
            return;
        }

        (*response) = (char*) malloc(sizeof(char) * (LOGOUT_OK_LEN + 1));
        sprintf(*response, "%s %s\n", LOGOUT_CMD, OK_STATUS);

        return;
        
    } else {

        // user is not logged in, check if user is registered

        char user_dir[strlen(USERS_DIR) + strlen(uid) + 2];
        sprintf(user_dir, "%s/%s", USERS_DIR, uid);

        if (stat(user_dir, &st) == 0){
            // user is registered but not logged in
            (*response) = (char*) malloc(sizeof(char) * (LOGOUT_NOK_LEN + 1));
            sprintf(*response, "%s %s\n", LOGOUT_CMD, NOK_STATUS);

            return;
        } else {
            // user is not registered
            (*response) = (char*) malloc(sizeof(char) * (LOGOUT_UNR_LEN + 1));
            sprintf(*response, "%s %s\n", LOGOUT_CMD, UNR_CMD);
            
            return;
        }
    }
}