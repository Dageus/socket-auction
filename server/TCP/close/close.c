#include "close.h"
#include "../../constants.h"
#include "../../common/common.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void process_close(int fd, char **response){

    char uid[UID_LEN + 1];
    char pwd[PWD_LEN + 1];
    char aid[AID_LEN + 1];

    read_word(fd, uid, UID_LEN + 1);

    if (uid == NULL || check_digits(uid) == -1) {
        /* wrong format */
        (*response) = (char*) malloc((CLOSE_CMD_ERR_LEN) * sizeof(char));
        sprintf((*response), "%s %s\n", CLOSE_CMD, ERR_STATUS);
        return;
    }

    read_word(fd, pwd, PWD_LEN + 1);

    if (pwd == NULL || check_alphanumeric(pwd) == -1) {
        /* wrong format */
        (*response) = (char*) malloc((CLOSE_CMD_ERR_LEN) * sizeof(char));
        sprintf((*response), "%s %s\n", CLOSE_CMD, ERR_STATUS);
        return;
    }

    read_word(fd, aid, AID_LEN + 1);

    if (aid == NULL || check_digits(aid) == -1) {
        /* wrong format */
        (*response) = (char*) malloc((CLOSE_CMD_ERR_LEN) * sizeof(char));
        sprintf((*response), "%s %s\n", CLOSE_CMD, ERR_STATUS);
        return;
    }
    
    // check if user is logged in

    char user_dir[strlen(USERS_DIR) + 2*strlen(uid) + strlen(LOGIN_SUFFIX) + 4];
    sprintf(user_dir, "%s/%s/%s%s", USERS_DIR, uid, uid, LOGIN_SUFFIX);

    struct stat st;

    if (stat(user_dir, &st) == 0){

        // user is logged in

        // check if auction exists and was started by user

        char auction_dir[strlen(AUCTIONS_DIR) + AID_LEN + 2];
        sprintf(auction_dir, "%s/%s", AUCTIONS_DIR, aid);

        if (stat(auction_dir, &st) == 0){

            // auction exists

            // check if it was started by user

            char start_file[strlen(auction_dir) + strlen(START_PREFIX) + AID_LEN + strlen(TXT_SUFFIX) + 2];
            sprintf(start_file, "%s/%s%s%s", auction_dir, START_PREFIX, aid, TXT_SUFFIX);

            // open start file

            FILE *fp = fopen(start_file, "r");

            if (fp == NULL) {
                fprintf(stderr, "Error opening start file\n");
                (*response) = (char*) malloc((CLOSE_CMD_ERR_LEN) * sizeof(char));
                sprintf(*response, "%s %s\n", CLOSE_CMD, ERR_STATUS);
                return;
            }

            // read start file

            char line[87];
            fgets(line, sizeof(line), fp);

            char *host_uid = strtok(line, " ");

            if (strcmp(host_uid, uid) != 0) {
                // auction was not started by user
                *response = (char*) malloc((CLOSE_CMD_EOW_LEN) * sizeof(char));
                sprintf(*response, "%s %s\n", CLOSE_CMD, EOW_STATUS);
                return;
            }

            // check if auction already closed

            char end_file[strlen(auction_dir) + strlen(END_PREFIX) + AID_LEN + strlen(TXT_SUFFIX) + 2];
            sprintf(end_file, "%s/%s%s%s", auction_dir, END_PREFIX, aid, TXT_SUFFIX);

            if (stat(end_file, &st) == 0) {
                // auction already closed
                *response = (char*) malloc((CLOSE_CMD_END_LEN) * sizeof(char));
                sprintf(*response, "%s %s\n", CLOSE_CMD, END_STATUS);
                return;
            }

            strtok(NULL, " ");
            strtok(NULL, " ");
            strtok(NULL, " ");
            
            time_t timeactive = atoi(strtok(NULL, " "));

            strtok(NULL, " ");

            time_t full_time = atoi(strtok(NULL, " "));

            // create end file

            char end_file_dir[strlen(auction_dir) + strlen(END_PREFIX) + AID_LEN + strlen(TXT_SUFFIX) + 2];

            sprintf(end_file_dir, "%s/%s%s%s", auction_dir, END_PREFIX, aid, TXT_SUFFIX);

            FILE *end_fp = fopen(end_file_dir, "w");

            if (end_fp == NULL) {
                fprintf(stderr, "Error creating end file\n");
                (*response) = (char*) malloc((CLOSE_CMD_ERR_LEN) * sizeof(char));
                sprintf(*response, "%s %s\n", CLOSE_CMD, ERR_STATUS);
                return;
            }

            // check if auction should've ended already

            time_t fulltime;
            struct tm *time_ended;
            char time_str[20];

            if (time(&fulltime) > timeactive + full_time) {
                // auction should've ended already
                // add the tfull_time to the timeactive
                full_time += timeactive;
                time_ended = gmtime(&full_time);
                sprintf(time_str, "%4d-%02d-%02d %02d:%02d:%02d", 
                    time_ended->tm_year + 1900, time_ended->tm_mon + 1, time_ended->tm_mday,
                    time_ended->tm_hour, time_ended->tm_min, time_ended->tm_sec);
                fprintf(end_fp, "%s %ld", time_str,  timeactive);
            } else {
                // auction ended before time
                time_ended = gmtime(&fulltime);
                timeactive = fulltime - full_time;
                sprintf(time_str, "%4d-%02d-%02d %02d:%02d:%02d", 
                    time_ended->tm_year + 1900, time_ended->tm_mon + 1, time_ended->tm_mday,
                    time_ended->tm_hour, time_ended->tm_min, time_ended->tm_sec);
                fprintf(end_fp, "%s %ld", time_str, timeactive);
            }

            fclose(end_fp);

            (*response) = (char*) malloc((CLOSE_CMD_OK_LEN) * sizeof(char));            
            sprintf(*response, "%s %s\n", CLOSE_CMD, OK_STATUS);

            return;

        } else {
            // auction does not exist
            *response = (char*) malloc((CLOSE_CMD_ERR_LEN) * sizeof(char));
            sprintf(*response, "%s %s\n", CLOSE_CMD, NO_AUCTION);
            return;
        }

    } else {
        // user is not logged in

        *response = (char*) malloc((CLOSE_CMD_NLG_LEN) * sizeof(char));
        sprintf(*response, "%s %s\n", CLOSE_CMD, NLG_STATUS);
        return;
    }
}
