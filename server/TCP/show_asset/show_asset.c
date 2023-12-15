#include "show_asset.h"
#include "../../constants.h"
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>


// int CheckAssetFile(char *aid) {

//     char filepath[13];
//     sprintf(filepath, "AUCTIONS/%s", aid);

//     struct stat filestat;
//     int ret_stat;
//     ret_stat = stat(fname, &filestat);
//     if (ret_stat == -1 || filestat.st_size == 0)
//         return 0;
//     return filestat.st_size;
// }

int process_show_asset(char* input,int fd){
    
    char* response = NULL;

    char *aid = strtok(input, " ");

    if (aid == NULL)
        return -1;

    if (strlen(aid) != AID_LEN){
        response  = (char*) malloc(SHOW_ASSET_ERR_LEN + 1);
        sprintf(response, "%s ERR\n", SHOW_ASSET_CMD);
        return -1;
    }

    char auction_dir[13];
    sprintf(auction_dir, "AUCTIONS/%s", aid);

    struct stat filestat;
    
    if (stat(auction_dir, &filestat) == -1) {
        response = (char*) malloc(SHOW_ASSET_ERR_LEN + 1) ;
        sprintf(response, "%s ERR\n", SHOW_ASSET_CMD);
        printf("File does not exist\n");
        return -1;
    }

    char start_file[27];
    sprintf(start_file, "AUCTIONS/%s/START_%s.txt", aid, aid);

    if (stat(start_file, &filestat) == -1){
        response = (char*) malloc(SHOW_ASSET_ERR_LEN + 1) ;
        sprintf(response, "%s ERR\n", SHOW_ASSET_CMD);
        printf("File does not exist\n");
        return -1;
    }

    FILE *fp = fopen(start_file, "r");
    if (fp == NULL){
        response = (char*) malloc(SHOW_ASSET_ERR_LEN + 1);
        sprintf(response, "%s ERR\n", SHOW_ASSET_CMD);
        return -1;
    }

    char start_file_content[filestat.st_size + 1];    // +1 for the null terminator

    // Check if memory allocation was successful
    if (start_file_content == NULL) {
        response = (char*) malloc(SHOW_ASSET_ERR_LEN + 1);
        sprintf(response, "%s ERR\n", SHOW_ASSET_CMD);
        perror("Memory allocation failed");
        return 1; // Exit with an error code
    }

    // Read file contents 

    if (fgets(start_file_content, filestat.st_size + 1, fp) == NULL) {
        response = (char*) malloc(SHOW_ASSET_ERR_LEN + 1);
        sprintf(response, "%s ERR\n", SHOW_ASSET_CMD);
        perror("Error reading file");
        return 1; // Exit with an error code
    }

    start_file_content[filestat.st_size] = '\0';

    strtok(start_file_content, " ");
    strtok(NULL, " ");
    char *fname = strtok(NULL, " ");    

    char filepath_file[13 + strlen(fname) + 1];
    sprintf(filepath_file, "AUCTIONS/%s/%s", aid, fname);

    struct stat filestat_file;

    if (stat(filepath_file, &filestat_file) == -1){
        response = (char*) malloc(SHOW_ASSET_ERR_LEN + 1) ;
        sprintf(response, "%s ERR\n", SHOW_ASSET_CMD);
        printf("File does not exist\n");
        return -1;
    }

    // Now I have to send the file to the client through the TCP socket

    response = (char*) malloc(sizeof(char) * (3 + 1 + 2 + strlen(fname) + 1 + 19 + 1));

    sprintf((response), "%s OK %s %ld", SHOW_ASSET_CMD, fname, filestat_file.st_size);

    // send the file to the client through the TCP socket

    char file_content_file[READ_WRITE_RATE]; 

    FILE *fp_send = fopen(filepath_file, "r");
    if (fp_send == NULL){
        response = (char*) malloc(SHOW_ASSET_ERR_LEN + 1);
        sprintf(response, "%s ERR\n", SHOW_ASSET_CMD);
        return -1;
    }

    if ((send(fd, response , READ_WRITE_RATE, 0)) == -1) {
            /*error*/
            fprintf(stderr, "Error sending message to server\n");
            free(response);
            close(fd);
            return -1;
        }

    while (fread(file_content_file, filestat_file.st_size, 1, fp_send) < sizeof(file_content_file)){
        if ((send(fd, file_content_file , READ_WRITE_RATE, 0)) == -1) {
            /*error*/
            fprintf(stderr, "Error sending message to server\n");
            close(fd);
            return -1;
        }
    }

    free(response);

}