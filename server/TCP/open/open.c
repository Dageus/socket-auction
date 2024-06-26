#include "open.h"
#include "../../constants.h"
#include "../../common/common.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <ctype.h>

char input[READ_WRITE_RATE];

int create_start_file(int aid, char* uid, char* name, char* fname, char* start_value, char* timeactive) {
    
    time_t fulltime;
    struct tm *current_time ;
    char time_str [20];
    char s_time_str [11];

    time(&fulltime);
    current_time = gmtime(&fulltime);

    sprintf(s_time_str, "%ld", (long)fulltime);
    sprintf(time_str, "%4d-%02d-%02d %02d:%02d:%02d", 
                                    current_time->tm_year + 1900, current_time->tm_mon + 1,
                                    current_time->tm_mday, current_time->tm_hour, current_time->tm_min, current_time->tm_sec);    
    
    
    char said[4];
    sprintf(said, "%03d", aid);

    char start_file[14];
    sprintf(start_file, "START_%s.txt", said);

    char start_dir[30];
    sprintf(start_dir, "AUCTIONS/%s/%s", said, start_file);

    FILE* fp = fopen(start_dir, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error creating start file\n");
        return -1;
    }

    fprintf(fp, "%s %s %s %s %s %s %s\n", uid, name, fname, start_value, timeactive, time_str, s_time_str);

    fclose(fp);
}

int create_auction_dir(int aid, char* uid, char* name, char* fname, char* start_value, char* timeactive) {
    char AID_dirname[15];
    char BIDS_dirname[20];
    int ret;

    if (aid < 1 || aid > 999)
        return 0;

    sprintf(AID_dirname, "AUCTIONS/%03d", aid);
    
    ret = mkdir(AID_dirname, 0700);
    if (ret == -1)
        /* error */
        return -1;
    
    sprintf(BIDS_dirname, "AUCTIONS/%03d/BIDS", aid);
    
    ret = mkdir(BIDS_dirname, 0700);
    if (ret == -1) {
        rmdir(AID_dirname);
        return -1;
    }

    if (create_start_file(aid, uid, name, fname, start_value, timeactive) == -1)
        return -1;    
    
    return 1;
}

void process_open_auction(int fd, int* aid, char** response){
    
    if (*aid >= 999){
        // reached the limit for auctions
        *response = (char*) malloc(sizeof(char) * OPEN_NOK_LEN);
        sprintf(*response, "%s %s\n", OPEN_RESPONSE, NOK_STATUS);
        return;
    }

    printf("[INFO]: Opening auction...\n");

    char uid[UID_LEN + 1];
    char pwd[PWD_LEN + 1];
    char name[MAX_NAME + 1];
    char start_value[8];
    char timeactive[6];
    char fname[MAX_FNAME_LEN + 1];
    char file_size_str[11];

    read_word(fd, uid, UID_LEN + 1);
    read_word(fd, pwd, PWD_LEN + 1);

    if (uid == NULL || pwd == NULL || strlen(uid) != UID_LEN || strlen(pwd) != PWD_LEN || check_digits(uid) == -1 || check_digits(pwd) == -1){
        *response = (char*) malloc(sizeof(char) * (OPEN_NOK_LEN));
        sprintf(*response, "%s %s\n", OPEN_RESPONSE, NOK_STATUS);
        close(fd);
        return;
    }

    // Check if user exists
    char user_dir[13];
    sprintf(user_dir, "USERS/%s", uid);

    struct stat st;

    if (stat(user_dir, &st) == -1){
        *response = (char*) malloc(sizeof(char) * (OPEN_NOK_LEN));
        sprintf(*response, "%s %s\n", OPEN_RESPONSE, NOK_STATUS);
        fprintf(stderr, "User does not exist\n");
        close(fd);
        return;
    }

    printf("User exists\n");

    // Check if password is correct
    if (check_password(user_dir, uid, pwd) == -1) {
        *response = (char*)malloc(sizeof(char) * (OPEN_NOK_LEN));
        sprintf(*response, "%s %s\n", OPEN_RESPONSE, NOK_STATUS);
        fprintf(stderr, "Password is incorrect\n");
        close(fd);
        return;
    }

    read_word(fd, name, MAX_NAME + 1);

    if (name == NULL || check_alphanumeric(name) == -1){
        *response = (char*)malloc(sizeof(char) * (OPEN_NOK_LEN));
        sprintf(*response, "%s %s\n", OPEN_RESPONSE, NOK_STATUS);
        fprintf(stderr, "Auction name is incorrect\n");
        close(fd);
        return;
    }

    read_word(fd, start_value, 6 + 1);

    if (start_value == NULL || check_digits(start_value) == -1){
        *response = (char*) malloc (sizeof(char) * (OPEN_NOK_LEN));
        sprintf(*response, "%s %s\n", OPEN_RESPONSE, NOK_STATUS);
        fprintf(stderr, "Start value is incorrect\n");
        close(fd);
        return;
    }
   
    read_word(fd, timeactive, 5 + 1);

    if (timeactive == NULL || check_digits(timeactive) == -1){
        *response = (char*) malloc (sizeof(char) * (OPEN_NOK_LEN));
        sprintf(*response, "%s %s\n", OPEN_RESPONSE, NOK_STATUS);
        fprintf(stderr, "Time active is incorrect\n");
        close(fd);
        return;
    }

    read_word(fd, fname, MAX_FNAME_LEN + 1);

    if (fname == NULL || check_alphanumeric(fname) == -1){
        *response = (char*) malloc (sizeof(char) * (OPEN_NOK_LEN));
        sprintf(*response, "%s %s\n", OPEN_RESPONSE, NOK_STATUS);
        fprintf(stderr, "File name is incorrect\n");
        close(fd);
        return;
    }

    read_word(fd, file_size_str, 10 + 1);

    if (file_size_str == NULL || check_digits(file_size_str) == -1){
        *response = (char*) malloc (sizeof(char) * (OPEN_NOK_LEN));
        sprintf(*response, "%s %s\n", OPEN_RESPONSE, NOK_STATUS);
        fprintf(stderr, "File size is incorrect\n");
        close(fd);
        return;
    }

    // Create directory for auction

    if (create_auction_dir(*aid, uid, name, fname, start_value, timeactive) == -1){
        *response = (char*) malloc(sizeof(char) * (OPEN_NOK_LEN));
        sprintf(*response, "%s %s\n", OPEN_RESPONSE, ERR_STATUS);
        fprintf(stderr, "Error creating auction directory\n");
        close(fd);
        return;
    }

    // Now I have to read from the socket chunk by chunk the rest of the bytes that contain the image

    int total_bytes_received = 0;
    int bytes_received;

    // create ASSET directory

    char asset_dir[20];
    sprintf(asset_dir, "AUCTIONS/%03d/ASSET", *aid);

    if (mkdir(asset_dir, 0700) == -1){
        *response = (char*) malloc(sizeof(char) * (OPEN_NOK_LEN));
        sprintf(*response, "%s %s\n", OPEN_RESPONSE, ERR_STATUS);
        fprintf(stderr, "Error creating asset directory\n");
        close(fd);
        return;
    }

    char dir_fname[strlen(AUCTIONS_DIR) + 1 + AID_LEN + 1 + strlen(ASSET) + 1 + strlen(fname) + 1];
    sprintf(dir_fname, "AUCTIONS/%03d/ASSET/%s", *aid, fname);

    FILE *file = fopen(dir_fname, "wb");
    
    if (!file) {
        *response = (char*)malloc(sizeof(char) * (OPEN_NOK_LEN));
        sprintf(*response, "%s %s\n", OPEN_RESPONSE, ERR_STATUS);
        fprintf(stderr, "Error opening file\n");
        fclose(file);
        close(fd);
        return;
    }


    while ( (size_t) total_bytes_received < atoi(file_size_str)) {
        
        bytes_received = read(fd, input, READ_WRITE_RATE);
        
        if (bytes_received < 0){
            *response = (char*) malloc(sizeof(char) * (OPEN_NOK_LEN));
            sprintf(*response, "%s %s\n", OPEN_RESPONSE, ERR_STATUS);
            fprintf(stderr, "Error reading from TCP socket\n");
            fclose(file);
            close(fd);
            return;
        }

        total_bytes_received += bytes_received;

        if (fwrite(input, 1 , bytes_received, file) != (size_t) bytes_received){
            *response = (char*)malloc(sizeof(char) * (OPEN_NOK_LEN));
            sprintf(*response, "%s %s\n", OPEN_RESPONSE, ERR_STATUS);     
            fprintf(stderr, "Error writing file\n");
            fclose(file);
            close(fd);
            return;
        
        } // Write the received bytes to the file
        
    }

    printf("total_bytes_received: %d\n", total_bytes_received);
    printf("bytes that should've been received: %d\n", atoi(file_size_str));

    fclose(file);

    // add auction to HOSTED auction of user

    char hosted_file[30];
    sprintf(hosted_file, "USERS/%s/HOSTED/%03d%s", uid, *aid, TXT_SUFFIX);

    FILE* fp = fopen(hosted_file, "w");

    if (fp == NULL){
        *response = (char*)malloc(sizeof(char) * (OPEN_NOK_LEN));
        sprintf(*response, "%s %s\n", OPEN_RESPONSE, ERR_STATUS);
        fprintf(stderr, "Error opening file\n");
        fclose(fp);
        close(fd);
        return;
    }

    fclose(fp);


    *response = (char*)malloc(sizeof(char) * (OPEN_OK_LEN));
    sprintf(*response, "%s %s %03d\n", OPEN_RESPONSE, OK_STATUS, *aid);
    
    printf("Auction opened with aid: %d\n", *aid);
    (*aid)++;

    return;
}