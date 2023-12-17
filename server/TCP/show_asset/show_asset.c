#include "show_asset.h"
#include "../../constants.h"
#include "../../common/common.h"
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

void process_show_asset(int fd){

    printf("Processing show asset\n");
    
    char aid[AID_LEN + 1];
    read_word(fd, aid, AID_LEN + 1);

    if (aid == NULL)
        return;

    if (strlen(aid) != AID_LEN){
        char response[SHOW_ASSET_ERR_LEN + 1];
        sprintf(response, "%s ERR\n", SHOW_ASSET_CMD);
        return;
    }

    char auction_dir[13];
    sprintf(auction_dir, "AUCTIONS/%s", aid);

    struct stat filestat;
    
    if (stat(auction_dir, &filestat) == -1) {
        char response[SHOW_ASSET_ERR_LEN + 1];
        sprintf(response, "%s ERR\n", SHOW_ASSET_CMD);
        printf("%s does not exist\n", auction_dir);
        return;
    }

    printf("Dir exists\n");

    char start_file[27];
    sprintf(start_file, "AUCTIONS/%s/START_%s.txt", aid, aid);

    printf("Start file: %s\n", start_file);

    if (stat(start_file, &filestat) == -1){
        char response[SHOW_ASSET_ERR_LEN + 1];
        sprintf(response, "%s ERR\n", SHOW_ASSET_CMD);
        printf("%s does not exist\n", start_file);
        return;
    }

    FILE *fp = fopen(start_file, "r");
    if (fp == NULL){
        char response[SHOW_ASSET_ERR_LEN + 1];
        sprintf(response, "%s ERR\n", SHOW_ASSET_CMD);
        return;
    }

    char start_file_content[filestat.st_size + 1];    // +1 for the null terminator

    // Check if memory allocation was successful
    if (start_file_content == NULL) {
        char response[SHOW_ASSET_ERR_LEN + 1];
        sprintf(response, "%s ERR\n", SHOW_ASSET_CMD);
        perror("Memory allocation failed");
        return;
    }

    // Read file contents 

    if (fgets(start_file_content, filestat.st_size + 1, fp) == NULL) {
        char response[SHOW_ASSET_ERR_LEN + 1];
        sprintf(response, "%s ERR\n", SHOW_ASSET_CMD);
        perror("Error reading file");
        return;
    }

    start_file_content[filestat.st_size] = '\0';

    strtok(start_file_content, " ");
    strtok(NULL, " ");
    char *fname = strtok(NULL, " ");    

    char filepath[19 + strlen(fname) + 1];
    sprintf(filepath, "AUCTIONS/%s/ASSET/%s", aid, fname);

    if (stat(filepath, &filestat) == -1){
        char response[SHOW_ASSET_ERR_LEN + 1];
        sprintf(response, "%s ERR\n", SHOW_ASSET_CMD);
        printf("File does not exist\n");
        return;
    }

    char response[READ_WRITE_RATE];
    sprintf(response, "%s OK %s %ld ", SHOW_ASSET_CMD, fname, filestat.st_size);

    printf("Response: %s\n", response);

    if (write(fd, response , strlen(response)) == -1) {
        /*error*/
        fprintf(stderr, "Error sending message to server\n");
        free(response);
        close(fd);
        return;
    }

    // send the file to the client through the TCP socket
    char file_content[READ_WRITE_RATE]; 

    FILE *file = fopen(filepath, "rb");
    if (!file) {
        fprintf(stderr, "Error opening file\n");
        close(fd);
        return;
    }

    ssize_t bytes_sent;
    size_t total_bytes_sent = 0;

    while ((bytes_sent = fread(file_content, 1, sizeof(file_content), file)) > 0) {
        printf("reading more data...\n");
        if ((bytes_sent = write(fd, file_content, bytes_sent)) == -1) {
            fprintf(stderr, "Error sending data\n");
            fclose(file);
            close(fd);
            return;
        }
        printf("wrote %ld bytes\n", bytes_sent);
        total_bytes_sent += bytes_sent;
    }

    printf("total_bytes_sent: %ld\n", total_bytes_sent);


}