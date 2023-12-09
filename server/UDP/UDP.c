#include "UDP.h"
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <bits/getopt_core.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

const char* UDP_commands[] = {
    "LIN",
    "LOU",
    "UNR",
    "LMA",
    "LMB",
    "LST",
    "SRC"
};

int UDP_cmd(char* cmd){
    int i;
    for (i = 0; i < 6; i++) {
        if (strcmp(cmd, UDP_commands[i]) == 0) {
            return 1;
        }
    }
    return -1;
}


typedef struct {


    // information regarding the UDP server functionality


} UDP;


