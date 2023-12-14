#include "open.h"
#include "../../constants.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void create_start_file(int aid, char* uid, char* name, char* fname, char* start_value, char* timeactive, char time_str[20], char s_time_str[11]) {
    char said[4];
    sprintf(said, "%03d", aid);

    char start_file[14];
    sprintf(start_file, "START_%s.txt", said);

    char start_dir[30];
    sprintf(start_dir, "AUCTIONS/%s/%s", said, start_file);

    FILE* fp = fopen(start_file, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error creating start file\n");
        return;
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
        return 0;
    
    sprintf(BIDS_dirname, "AUCTIONS/%03d/BIDS", aid);
    
    ret = mkdir(BIDS_dirname, 0700);
    if (ret == -1) {
        rmdir(AID_dirname);
        return 0;
    }

    create_start_file(aid, uid, name, fname, start_value, timeactive, time_str, s_time_str);
    
    
    return 1;
}

int process_open_auction(int fd, int aid){


    
    if (aid >= 999)
        // * reached the limit for auctions
        return 0; // * aid is the global counter for aid's

    ssize_t n;

    char input[READ_WRITE_RATE];

    // read from soccket fd 20 bytes

    if((n = read(fd, input, READ_WRITE_RATE)) == -1){
        fprintf(stderr, "Error reading from TCP socket\n");
        exit(1);
    }

    char* uid = strtok(input, " ");
    char* pwd = strtok(NULL, " ");
    char* name  = strtok(NULL, " ");
    char* start_value = strtok(NULL, " ");
    char* timeactive = strtok(NULL, " ");
    char* fname = strtok(NULL, " ");
    size_t file_size = atoi(strtok(NULL, " "));
    char* img = strtok(NULL, " ");

    size_t total_bytes_received = 0;
    size_t bytes_received;
    // Now I have to read from the socket chunk by chunk the rest of the bytes that contain the image
    FILE *file = fopen(fname, "wb");
    
    if (!file) {
        fprintf(stderr, "Error opening file\n");
        fclose(file);
        close(fd);
        return -1;
    }

    if(img != NULL){
        if (fwrite(img, 1, strlen(img), file) != strlen(img)) {
            fprintf(stderr, "Error writing file\n");
            fclose(file);
            close(fd);
            return -1;
        }
        total_bytes_received += strlen(img);
    }

    while (total_bytes_received < file_size) {
        
        bytes_received = read(fd, input, 512);
        
        if (bytes_received < 0){
            fprintf(stderr, "Error reading from TCP socket\n");
            fclose(file);
            close(fd);
            return -1;
        }

        total_bytes_received += bytes_received;

        if(fwrite(input, 1 , bytes_received, file) != bytes_received){
            fprintf(stderr, "Error writing file\n");
            fclose(file);
            close(fd);
            return -1;
        
        } // Write the received bytes to the file
        
    }

    fclose(file);

    CreateAUCTIONDir(aid, uid, name, fname, start_value, timeactive);

    char img_dir[38];
    sprintf(img_dir, "AUCTIONS/%03d/%s", aid, fname);
    if(rename(fname, img_dir) == -1){
        fprintf(stderr, "Error renaming file\n");
        close(fd);
        return -1;
    }
    return 0;
}