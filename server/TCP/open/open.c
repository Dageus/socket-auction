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

int process_open_auction(int fd, int aid, char** response){
    
    if (aid >= 999)
        // reached the limit for auctions
        return 0;  

    printf("processing open auction...\n");

    char uid[UID_LEN + 1];
    char pwd[PWD_LEN + 1];
    char name[MAX_NAME + 1];
    char start_value[8];
    char timeactive[6];
    char fname[MAX_FNAME_LEN + 1];
    char file_size_str[11];

    read_word(fd, uid, UID_LEN + 1);
    read_word(fd, pwd, PWD_LEN + 1);
    read_word(fd, name, MAX_NAME + 1);
    read_word(fd, start_value, 7 + 1);
    read_word(fd, timeactive, 5 + 1);
    read_word(fd, fname, MAX_FNAME_LEN + 1);
    read_word(fd, file_size_str, 10 + 1);

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

    printf("User exists\n");

    // Check if password is correct
    if (check_password(user_dir, uid, pwd) == -1){
        *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
        sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
        fprintf(stderr, "Password is incorrect\n");
        close(fd);
        return -1;
    }

    // Check if auction name is valid

    if (strlen(name) > 10){
        *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
        sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
        fprintf(stderr, "Auction name is too long\n");
        close(fd);
        return -1;
    }

    printf("Auction name is valid\n");

    // Create directory for auction

    if (create_auction_dir(aid, uid, name, fname, start_value, timeactive) == -1){
        *response = (char*) malloc(sizeof(char) * (3 + 1 + 3 + 1));
        sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
        fprintf(stderr, "Error creating auction directory\n");
        close(fd);
        return -1;
    }

    // Now I have to read from the socket chunk by chunk the rest of the bytes that contain the image

    int total_bytes_received = 0;
    int bytes_received;
    char dir_fname[strlen(AUCTIONS_DIR) + 1 + AID_LEN + 1 + strlen(ASSET) + 1 + strlen(fname) + 1];

    // create ASSET directory

    char asset_dir[20];
    sprintf(asset_dir, "AUCTIONS/%03d/ASSET", aid);

    if (mkdir(asset_dir, 0700) == -1){
        *response = (char*) malloc(sizeof(char) * (3 + 1 + 3 + 1));
        sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
        fprintf(stderr, "Error creating asset directory\n");
        close(fd);
        return -1;
    }

    sprintf(dir_fname, "AUCTIONS/%03d/ASSET/%s", aid, fname);

    FILE *file = fopen(dir_fname, "wb");

    printf("file to write to: %s\n", dir_fname);
    
    if (!file) {
        *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
        sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
        fprintf(stderr, "Error opening file\n");
        fclose(file);
        close(fd);
        return -1;
    }

    printf("File opened\n");

    printf("total_bytes_received: %d\n", total_bytes_received);


    while ( (size_t) total_bytes_received < atoi(file_size_str)) {
        
        bytes_received = read(fd, input, READ_WRITE_RATE);
        
        if (bytes_received < 0){
            *response = (char*) malloc(sizeof(char) * (3 + 1 + 3 + 1));
            sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
            fprintf(stderr, "Error reading from TCP socket\n");
            fclose(file);
            close(fd);
            return -1;
        }

        printf("\n");
        printf("received %d bytes\n", bytes_received);
        printf("\n");

        total_bytes_received += bytes_received;

        printf("total_bytes_received: %d\n", total_bytes_received);

        if (fwrite(input, 1 , bytes_received, file) != (size_t) bytes_received){
            *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
            sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);     
            fprintf(stderr, "Error writing file\n");
            fclose(file);
            close(fd);
            return -1;
        
        } // Write the received bytes to the file
        
    }

    printf("\n");
    printf("received %d bytes\n", total_bytes_received);
    printf("\n");

    printf("total_bytes_received: %d\n", total_bytes_received);

    fclose(file);

    printf("File closed\n");

    // add auction to HOSTED auction of user

    char hosted_file[30];
    sprintf(hosted_file, "USERS/%s/HOSTED/%03d%s", uid, aid, TXT_SUFFIX);

    FILE* fp = fopen(hosted_file, "w");

    if (fp == NULL){
        *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
        sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
        fprintf(stderr, "Error opening file\n");
        fclose(fp);
        close(fd);
        return -1;
    }

    fclose(fp);


    *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1 + 3 + 1 ));
    sprintf(*response, "%s %s %03d", OPEN_RESPONSE, OK_STATUS, aid);
    
    return 0;
}

/*


FUNCOES QUE PARTIRAM O CODIGO TODO

FAZEM COM QUE O FILE DESCRIPTOR SE FECHE A TOA E PARTE ISTO TUDO

*/

// if (uid == NULL || pwd == NULL || name == NULL || start_value == NULL || timeactive == NULL || fname == NULL || file_size == 0) {
    //     *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
    //     sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
    //     fprintf(stderr, "Error reading from TCP socket\n");
    //     close(fd);
    //     return -1;
    // }

    // if (strlen(uid) != UID_LEN || strlen(pwd) != PWD_LEN || strlen(name) > 10 ||
    //     strlen(start_value) > 6 || strlen(timeactive) > 5 || strlen(fname) > 24 || file_size > 10000000) {
    //     *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
    //     sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
    //     fprintf(stderr, "Error reading from TCP socket\n");
    //     close(fd);
    //     return -1;
    // }

    // for (int i = 0; i < (int) strlen(name); i++){
    //     if (!isalnum(name[i]) || name[i] == '_' || name[i] == '-' || name[i] == '.'){
    //         *response = (char*) malloc (sizeof(char) * (3 + 1 + 3 + 1));
    //         sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
    //         fprintf(stderr, "Auction name is not alphanumeric\n");
    //         close(fd);
    //         return -1;
    //     }
    // }

    // for (int i = 0; i < (int) strlen(start_value); i++){
    //     if (!isdigit(start_value[i])){
    //         *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
    //         sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
    //         fprintf(stderr, "Start value is not numeric\n");
    //         close(fd);
    //         return -1;
    //     }
    // }

    // for (int i = 0; i < (int) strlen(timeactive); i++){
    //     if (!isdigit(timeactive[i])){
    //         *response = (char*)malloc(sizeof(char) * (3 + 1 + 3 + 1));
    //         sprintf(*response, "%s %s", OPEN_RESPONSE, NOK_STATUS);
    //         fprintf(stderr, "Time active is not numeric\n");
    //         close(fd);
    //         return -1;
    //     }
    // }

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