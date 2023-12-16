#include "../constants.h"
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_password(char* user_dir, char* uid, char* pwd){

    char pwd_file[strlen(uid) + strlen(PWD_SUFFIX) + 1];
    sprintf(pwd_file, "%s%s", uid, PWD_SUFFIX);

    char pwd_dir[strlen(user_dir) + strlen(pwd_file) + 2];
    sprintf(pwd_dir, "%s/%s", user_dir, pwd_file);

    FILE* fp = fopen(pwd_dir, "r");

    if (fp == NULL) {
        fprintf(stderr, "Error opening password file\n");
        return -1;
    }

    char pwd_read[PWD_LEN + 1];
    if (fgets(pwd_read, PWD_LEN + 1, fp) == NULL) {
        fprintf(stderr, "Error reading password file\n");
        fclose(fp);
        return -1;
    }

    if (strcmp(pwd_read, pwd) == 0){
        printf("password is correct\n");
        fclose(fp);
        return 0;
    }
    else {
        printf("password is different\n");
        fclose(fp);
        return -1;
    }
}