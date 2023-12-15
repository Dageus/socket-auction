#ifndef TCP_H
#define TCP_H

#include <stdio.h>

#define FILE_TO_BE_SENT 1
#define FILE_TO_BE_RECEIVED 0


typedef struct file {
    int code;
    char* filename;
} file;

typedef struct TCP_response {
    file* file;
    char* msg;
} TCP_response;


int TCP_cmd(char* cmd);
int send_TCP(TCP_response* response, char* ip, char* port);

#endif // TCP_H