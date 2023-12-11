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
#include "UDP/UDP.h"

int process_unregister(char* input, UDP_response** response){
    char* uid = strtok(input, " ");
    char* pwd = strtok(NULL, " ");

    if (strlen(uid) != UID_LEN || strlen(pwd) != PWD_LEN)
        /* wrong format */
        return 0;

    char* login_dir = (char*) malloc((strlen(USERS_DIR) + strlen(LOGIN_SUFFIX) + 2*strlen(uid) + 3) * sizeof(char));
    char* pwd_dir = (char*) malloc((strlen(USERS_DIR) + strlen(LOGIN_SUFFIX) + 2*strlen(uid) + 3) * sizeof(char));

    sprintf(login_dir, "%s/%s/%s%s", USERS_DIR, uid, uid, LOGIN_SUFFIX);
    sprintf(pwd_dir, "%s/%s/%s%s", USERS_DIR, uid, uid, PWD_SUFFIX);

    // find directory
    struct stat st;

    if (stat(login_dir, &st) == 0) {
        // remove file
        if (unlink(pwd_dir) == -1) {
            printf("Error removing login file\n");
            free(login_dir);
            free(pwd_dir);
            return -1;
        }

        // remove login file as well
        if (unlink(login_dir) == -1) {
            printf("Error removing login file\n");
            free(login_dir);
            free(pwd_dir);
            return -1;
        }
    } else {
        // directory doesn't exist
        free(login_dir);
        free(pwd_dir);
        return -1;
    }
}