#ifndef TCP_H
#define TCP_H


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
