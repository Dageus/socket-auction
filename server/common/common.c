#include "../constants.h"
#include "common.h"

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

int read_word(int fd, char *buffer, int size) {
    int i = 0;
    while (i < size)
    {
        if (read(fd, buffer + i, sizeof(char)) == -1)
        {
            printf("Error while reading from socket.\n");
            buffer = NULL;
            return -1;
        }
        if (buffer[i] == ' ' || buffer[i] == '\n')
        {
            memset(buffer + i, '\0', 1);
            break;
        }
        i++;
    }
    return 0;
}

int get_global_aid_number(){

    char auctions_dir[strlen(AUCTIONS_DIR) + 1];
    sprintf(auctions_dir, "%s", AUCTIONS_DIR);

    struct dirent **namelist;
    int n_entries;
    int aid = 0;

    n_entries = scandir(auctions_dir, &namelist, NULL, alphasort);

    while (n_entries--) {
        if (strcmp(namelist[n_entries]->d_name, ".") == 0 || strcmp(namelist[n_entries]->d_name, "..") == 0) {
            continue;
        }

        aid = atoi(namelist[n_entries]->d_name);
        break;
    }

    return ++aid;
}

int check_auction_end(int aid){

    char pathname[strlen(AUCTIONS_DIR) + AID_LEN + 2];
    sprintf(pathname, "%s/%d", AUCTIONS_DIR, aid);

    char start_file[strlen(pathname) + strlen(START_PREFIX) + AID_LEN + strlen(TXT_SUFFIX) + 2];
    sprintf(start_file, "%s/%s%d%s", pathname, START_PREFIX, aid, TXT_SUFFIX);

    FILE* fp = fopen(start_file, "r");

    if (fp == NULL) {
        fprintf(stderr, "Error opening auction file\n");
        return -1;
    }

    char line[87];
    if (fgets(line, 87, fp) == NULL) {
        fprintf(stderr, "Error reading auction file\n");
        fclose(fp);
        return -1;
    }

    char* host_uid = strtok(line, " ");

    strtok(NULL, " ");
    strtok(NULL, " ");
    strtok(NULL, " ");
    
    time_t timeactive = atoi(strtok(NULL, " "));

    strtok(NULL, " ");

    time_t full_time = atoi(strtok(NULL, " "));

    // now to check if the auction should've ended already or not

    time_t fulltime;
    struct tm * timeinfo;

    if (time(&fulltime) > timeactive + full_time) {
        // auction should've ended already
        // add the tfull_time to the timeactive

        char time_str[20];

        full_time += timeactive;
        timeinfo = gmtime(&full_time);
        sprintf(time_str, "%4d-%02d-%02d %02d:%02d:%02d", 
            timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
            timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

        // create the end file

        char end_file[strlen(pathname) + strlen(END_PREFIX) + AID_LEN + strlen(TXT_SUFFIX) + 2];
        sprintf(end_file, "%s/%s%03d%s", pathname, END_PREFIX, aid, TXT_SUFFIX);

        FILE *end_fp = fopen(end_file, "w");

        if (end_fp == NULL) {
            fprintf(stderr, "Error creating end file\n");
            fclose(fp);
            return -1;
        }

        fprintf(end_fp, "%s %ld\n", time_str,  timeactive);

        fclose(end_fp);

        fclose(fp);

        return 1;
    }

    fclose(fp);

    return 0;
}