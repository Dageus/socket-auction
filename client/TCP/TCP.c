#include "TCP.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *TCP_commands[] = {
    "open",
    "close",
    "show_asset",
    "sa",
    "bid",
    "b"
};


int TCP_cmd(char* cmd){
    int i;
    for (i = 0; i < 6; i++) {
        if (strcmp(cmd, TCP_commands[i]) == 0)
            return 1;
    }
    return -1;
}

int create_TCP(){

    return 0;
}

int send_TCP(){


    return 0;
}