#ifndef TCP_H
#define TCP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../constants.h"
#include "open/open.h"
#include "close/close.h"
#include "bid/bid.h"
#include "show_asset/show_asset.h"


int TCP_cmd(char *cmd);

typedef struct file {
    int code;
    char* filename;
} file;

typedef struct TCP_response {
    file* file;
    char* msg;
} TCP_response;

// 



#endif // TCP_H
