#include "open.h"
#include "../../constants.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>




void create_start_file(int aid, char* uid, char* name, char* fname, char* start_value, char* timeactive) {
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

    fprintf(fp, "%s %s %s %s %s\n", uid, name, fname, start_value, timeactive);

    fclose(fp);
}

int CreateAUCTIONDir(int aid, char* name, char* fname, char* start_value, char* timeactive) {
    char AID_dirname[15];
    char BIDS_dirname[20];
    int ret;

    if (aid < 1 || aid > 999)
        return 0;

    aid += 1;
    
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

    create_start_file(aid, name, fname, start_value, timeactive);
    
    
    return 1;
}


int process_open_auction(int fd, int aid){
    
    if (aid >= 999)
        // * reached the limit for auctions
        return 0; // * aid is the global counter for aid's

    ssize_t n;

    char input[45];

    // read from soccket fd 20 bytes

    if(n = read(fd, input, 45) == -1)
        fprintf(stderr, "Error reading from TCP socket\n");
        exit(1);

    char* uid = strtok(input, " ");
    char* pwd = strtok(NULL, " ");
    char* name  = strtok(NULL, " ");
    char* start_value = strtok(NULL, " ");
    char* timeactive = strtok(NULL, " ");
    char* fname = strtok(NULL, " ");
    char buffer[512];

    size_t file_size = atoi(strtok(NULL, " "));
    size_t total_bytes_received = 0;
    size_t bytes_received;
    // Now I have to read from the socket chunk by chunk the rest of the bytes that contain the image
    FILE *file = fopen(fname, "wb");
    
    if (!file) {
        fprintf(stderr, "Error opening file\n");
        close(fd);
        return -1;
    }

    while (total_bytes_received < file_size && (bytes_received = read(fd, buffer, 512)) > 0) {
        if(bytes_received < 0){
            fprintf(stderr, "Error reading from TCP socket\n");
            fclose(file);
            close(fd);
            return -1;
        }            
        fwrite(buffer, 1, bytes_received, file);  // Write the received bytes to the file
        total_bytes_received += bytes_received;
    }

    fclose(file);

    CreateAUCTIONDir(aid, name, fname, start_value, timeactive);

    char img_dir[38];
    sprintf(img_dir, "AUCTIONS/%03d/%s", aid, fname);
    if(rename(fname, img_dir) == -1)
        fprintf(stderr, "Error renaming file\n");
        close(fd);
        return -1;


    return 0;
}