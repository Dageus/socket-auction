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

int process_unregister(char* input){
    char* uid = (char*) malloc(UID_LENGTH * sizeof(char));
    char* pwd = (char*) malloc(PASSWORD_LEN * sizeof(char));

    uid = strtok(input, " ");
    pwd = strtok(NULL, " ");

    char* login_dir = (char*) malloc((strlen(USERS_DIR) + strlen(LOGIN_SUFFIX) + 2*strlen(uid) + 3) * sizeof(char));
    char* pwd_dir = (char*) malloc((strlen(USERS_DIR) + strlen(LOGIN_SUFFIX) + 2*strlen(uid) + 3) * sizeof(char));

    sprintf(login_dir, "%s/%s/%s%s", USERS_DIR, uid, uid, LOGIN_SUFFIX);
    sprintf(pwd_dir, "%s/%s/%s%s", USERS_DIR, uid, uid, PWD_SUFFIX);

    // find directory
    struct stat st;

    // ! checkar se tambem e preciso retirar o ficheiro do login.txt

    if (stat(login_dir, &st) == 0) {
        // remove file
        if (unlink(pwd_dir) == -1) {
            printf("Error removing login file\n");
            return -1;
        }
    } else
        // directory doesn't exist
        return -1;

}