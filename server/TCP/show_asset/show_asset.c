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

    char filepath[13];
    sprintf(filepath, "AUCTIONS/%s", aid);
    size_t file_len;

    struct stat filestat;
    
    if (stat(filepath, &filestat) == -1){
        response = (char*) malloc(SHOW_ASSET_ERR_LEN + 1) ;
        sprintf(response, "%s ERR\n", SHOW_ASSET_CMD);
        printf("File does not exist\n");
        return -1;
    }

    char filepath_START[27];
    sprintf(filepath_START, "AUCTIONS/%s/START_%s.txt", aid, aid);

    struct stat filestat_START;

    if (stat(filepath, &filestat) == -1){
        response = (char*) malloc(SHOW_ASSET_ERR_LEN + 1) ;
        sprintf(response, "%s ERR\n", SHOW_ASSET_CMD);
        printf("File does not exist\n");
        return -1;
    }

    char *filename = malloc(filestat_START.st_size + 1);
    FILE *fp = fopen(filepath, "r");
    if (fp == NULL){
        response = (char*) malloc(SHOW_ASSET_ERR_LEN + 1);
        sprintf(response, "%s ERR\n", SHOW_ASSET_CMD);
        return -1;
    }

    char *file_content = (char *)malloc(filestat.st_size + 1); // +1 for the null terminator

    // Check if memory allocation was successful
    if (file_content == NULL) {
        response = (char*) malloc(SHOW_ASSET_ERR_LEN + 1);
        sprintf(response, "%s ERR\n", SHOW_ASSET_CMD);
        perror("Memory allocation failed");
        free(file_content);
        return 1; // Exit with an error code
    }

    // Read file contents 

    if(fread(file_content, filestat.st_size, 1, fp) == -1){
        response = (char*) malloc(SHOW_ASSET_ERR_LEN + 1);
        sprintf(response, "%s NOK\n", SHOW_ASSET_CMD);
        return -1;
    }

    file_content[filestat.st_size] = '\0';

    char *uid = strtok(file_content, " ");
    char *name = strtok(NULL, " ");
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

    char *file_content_file = (char *)malloc(READ_WRITE_RATE); 

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

    while(fread(file_content_file, filestat_file.st_size, 1, fp_send) == -1){
        if ((send(fd, file_content_file , READ_WRITE_RATE, 0)) == -1) {
            /*error*/
            fprintf(stderr, "Error sending message to server\n");
            free(file_content_file);
            close(fd);
            return -1;
        }
    }

    free(response);
    free(file_content);
    free(file_content_file);

}