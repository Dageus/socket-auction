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

int create_start_file(int aid, char* uid, char* name, char* fname, char* start_value, char* timeactive, char time_str[20], char s_time_str[11]) {
    char said[4];
    sprintf(said, "%03d", aid);

    char start_file[14];
    sprintf(start_file, "START_%s.txt", said);

    char start_dir[30];
    sprintf(start_dir, "AUCTIONS/%s/%s", said, start_file);

    FILE* fp = fopen(start_file, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error creating start file\n");
        return -1;
    }

    fprintf(fp, "%s %s %s %s %s %s %s\n", uid, name, fname, start_value, timeactive, time_str, s_time_str);

    fclose(fp);
}

int CreateAUCTIONDir(int aid, char* uid, char* name, char* fname, char* start_value, char* timeactive) {
    char AID_dirname[15];
    char BIDS_dirname[20];
    int ret;

    time_t fulltime;
    struct tm *current_time ;
    char time_str [20];
    char s_time_str [11];

    if (aid < 1 || aid > 999)
        return 0;

    aid += 1;

    time(&fulltime);
    current_time = gmtime(&fulltime);

    sprintf(s_time_str, "%ld", (long)fulltime);
    sprintf(time_str, "%4d-%02d-%02d %02d:%02d:%02d", 
                                    current_time->tm_year + 1900, current_time->tm_mon + 1,
                                    current_time->tm_mday, current_time->tm_hour, current_time->tm_min, current_time->tm_sec);    
    
    sprintf(AID_dirname, "AUCTIONS/%03d", aid);
    
    ret = mkdir(AID_dirname, 0700);
    if (ret == -1)
        
        return -1;
    
    sprintf(BIDS_dirname, "AUCTIONS/%03d/BIDS", aid);
    
    ret = mkdir(BIDS_dirname, 0700);
    if (ret == -1) {
        rmdir(AID_dirname);
        return -1;
    }

    if(create_start_file(aid, uid, name, fname, start_value, timeactive, time_str, s_time_str) == -1)
        return -1;    
    
    return 1;
}

int process_open_auction(int fd, int aid, char** response){

    printf("Processing open auction\n");
    
    if (aid >= 999)
        // * reached the limit for auctions
        return 0; // * aid is the global counter for aid's

    ssize_t n;

    char input[READ_WRITE_RATE];

    printf("Reading from TCP socket\n");

    if ((n = read(fd, input, READ_WRITE_RATE)) == -1){
        fprintf(stderr, "Error reading from TCP socket\n");
        exit(1);
    }

    printf("Read from TCP socket\n");
    input[n] = '\0';
    printf("input: %s\n", input);

    char* uid = strtok(input, " ");
    char* pwd = strtok(NULL, " ");
    char* name  = strtok(NULL, " ");
    char* start_value = strtok(NULL, " ");
    char* timeactive = strtok(NULL, " ");
    char* fname = strtok(NULL, " ");
    size_t file_size = atoi(strtok(NULL, " "));
    char* img = strtok(NULL, " ");

    printf("uid: %s\n", uid);
    printf("pwd: %s\n", pwd);
    printf("name: %s\n", name);
    printf("start_value: %s\n", start_value);
    printf("timeactive: %s\n", timeactive);
    printf("fname: %s\n", fname);
    printf("file_size: %ld\n", file_size);
    printf("img: %s\n", img);

    printf("sleeping to rethink my life\n");
    sleep(5);
    close(fd);
    printf("killing parent\n");
    kill(getppid(), SIGUSR1);

    if (uid == NULL || pwd == NULL || name == NULL || start_value == NULL || timeactive == NULL || fname == NULL || file_size == 0) {
        *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
        sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
        fprintf(stderr, "Error reading from TCP socket\n");
        close(fd);
        return -1;
    }

    if (strlen(uid) != UID_LEN || strlen(pwd) != PWD_LEN || strlen(name) > 10 ||
        strlen(start_value) > 6 || strlen(timeactive) > 5 || strlen(fname) > 24 || file_size > 10000000) {
        *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
        sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
        fprintf(stderr, "Error reading from TCP socket\n");
        close(fd);
        return -1;
    }

    for (int i = 0; i < (int) strlen(name); i++){
        if (!isalnum(name[i]) || name[i] == '_' || name[i] == '-' || name[i] == '.'){
            *response = (char*) malloc (sizeof(char) * (3 + 1 + 3 + 1));
            sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
            fprintf(stderr, "Auction name is not alphanumeric\n");
            close(fd);
            return -1;
        }
    }

    for (int i = 0; i < (int) strlen(start_value); i++){
        if (!isdigit(start_value[i])){
            *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
            sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
            fprintf(stderr, "Start value is not numeric\n");
            close(fd);
            return -1;
        }
    }

    for (int i = 0; i < (int) strlen(timeactive); i++){
        if (!isdigit(timeactive[i])){
            *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
            sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
            fprintf(stderr, "Time active is not numeric\n");
            close(fd);
            return -1;
        }
    }

    //check if image name is alphanumeric and if after the dot the len is 3

    // for (int i = 0; i < (int) strlen(fname); i++){

    //     printf("fname[%d]: %c\n", i, fname[i]);

    //     if(!isalnum(fname[i]) ){
    //         *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
    //         sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
    //         fprintf(stderr, "Image name is not alphanumeric\n");
    //         close(fd);
    //         return -1;
    //     }

    //     else if(fname[strlen(fname) - 4] != '.'){
    //         *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
    //         sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
    //         fprintf(stderr, "Not a file\n");
    //         close(fd);
    //         return -1;
    //     }

    //     else if(!isalnum(fname[strlen(fname) - 3]) || !isalnum(fname[strlen(fname) - 2]) || !isalnum(fname[strlen(fname) - 1])){
    //         *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
    //         sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
    //         fprintf(stderr, "fily type not alphanumeric\n");
    //         close(fd);
    //         return -1;
    //     }
    // }


    // Check if user exists

    char user_dir[13];
    sprintf(user_dir, "USERS/%s", uid);

    struct stat st;

    if (stat(user_dir, &st) == -1){
        *response = (char*) malloc(sizeof(char) * (3 + 1 + 3 + 1));
        sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
        fprintf(stderr, "User does not exist\n");
        close(fd);
        return -1;
    }

    // Check if password is correct
    if (check_password(user_dir, uid, pwd) == -1){
        *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
        sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
        fprintf(stderr, "Password is incorrect\n");
        close(fd);
        return -1;
    }

    // Check if auction name is valid

    if (strlen(name) > 24){
        *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
        sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
        fprintf(stderr, "Auction name is too long\n");
        close(fd);
        return -1;
    }

    // Check if start value is valid

    int total_bytes_received = 0;
    int bytes_received;
    // Now I have to read from the socket chunk by chunk the rest of the bytes that contain the image
    FILE *file = fopen(fname, "wb");
    
    if (!file) {
        *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
        sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
        fprintf(stderr, "Error opening file\n");
        fclose(file);
        close(fd);
        return -1;
    }

    if(img != NULL){
        if (fwrite(img, 1, strlen(img), file) != strlen(img)) {
            *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
            sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
            fprintf(stderr, "Error writing file\n");
            fclose(file);
            close(fd);
            return -1;
        }
        total_bytes_received += strlen(img);
    }

    while ( (size_t) total_bytes_received < file_size) {
        
        bytes_received = read(fd, input, 512);
        
        if (bytes_received < 0){
            *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
            sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
            fprintf(stderr, "Error reading from TCP socket\n");
            fclose(file);
            close(fd);
            return -1;
        }

        total_bytes_received += bytes_received;

        if(fwrite(input, 1 , bytes_received, file) != (size_t) bytes_received){
            *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
            sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);     
            fprintf(stderr, "Error writing file\n");
            fclose(file);
            close(fd);
            return -1;
        
        } // Write the received bytes to the file
        
    }

    fclose(file);

    if (CreateAUCTIONDir(aid, uid, name, fname, start_value, timeactive) == -1){
        *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
        sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
        fprintf(stderr, "Error creating auction directory\n");
        close(fd);
        return -1;
    }

    char img_dir[38];
    sprintf(img_dir, "AUCTIONS/%03d/%s", aid, fname);
    if (rename(fname, img_dir) == -1){
        *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
        sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
        fprintf(stderr, "Error renaming file\n");
        close(fd);
        return -1;
    }

    *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1 + 3 + 1 ));
    sprintf(*response, "%s %s %03d", OPEN_RESPONSE, OK_STATUS, aid);
    
    return 0;
}