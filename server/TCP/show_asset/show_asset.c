#include "show_asset.h"
#include "../../constants.h"
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int CheckAssetFile(char *fname) {
    struct stat filestat;
    int ret_stat;
    ret_stat = stat(fname, &filestat);
    if (ret_stat == -1 || filestat.st_size == 0)
        return 0;
    return filestat.st_size;
}

int process_show_asset(char* input, char** response){

    char *aid = strtok(input, " ");

    if (aid == NULL)
        return 0;

    if (strlen(aid) != AID_LEN){
        (*response) = (char*) malloc(SHOW_ASSET_ERR_LEN + 1);
        sprintf((*response), "%s ERR\n", SHOW_ASSET_CMD);
        return -1;
    }


}