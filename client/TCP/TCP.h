#ifndef TCP_H
#define TCP_H

#include <stdio.h>

#define FILE_SENT 1
#define FILE_NOT_REQUIRED 0
#define FILE_RECEIVED 2

typedef struct TCP_response {
    int code;
    char* filename;
    char* msg;
} TCP_response;


int TCP_cmd(char* cmd);
int send_TCP(TCP_response* response);

#endif // TCP_H