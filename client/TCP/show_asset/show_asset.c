#include "show_asset.h"
#include "../../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int process_show_asset(char* input, char* uid, char* pwd, char* response){

    char* n_cmd = strtok(input, " ");
    char* asset_id = strtok(NULL, " ");

    if (asset_id == NULL){
        fprintf(stderr, "error: asset_id\n");
        return -1;
    }

    sprintf(response, "%s %s %s %s", n_cmd, uid, pwd, asset_id);


    return 0;
}