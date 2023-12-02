#include "show_asset.h"
#include "../../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int process_show_asset(char* input, char** response, int* code){

    *response = (char *) malloc(sizeof(char) * SHOW_ASSET_LEN);

    char* n_cmd = strtok(input, " ");
    char* asset_id = strtok(NULL, " ");

    if (asset_id == NULL){
        fprintf(stderr, "error: asset_id\n");
        return -1;
    }

    sprintf(*response, "%s %s\n", n_cmd, asset_id);


    return 0;
}