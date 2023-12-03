#include "show_asset.h"
#include "../TCP.h"
#include "../../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int process_show_asset(char* input, TCP_response** response){

    (*response)->msg = (char *) malloc(sizeof(char) * SHOW_ASSET_LEN);
    (*response)->code = FILE_RECEIVED;
    (*response)->filename = NULL;

    char* n_cmd = strtok(input, " ");
    char* asset_id = strtok(NULL, " ");

    if (asset_id == NULL){
        fprintf(stderr, "error: asset_id\n");
        return -1;
    }

    sprintf((*response)->msg, "%s %s\n", n_cmd, asset_id);


    return 0;
}