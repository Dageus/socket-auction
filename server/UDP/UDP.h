#ifndef UDP_H
#define UDP_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int UDP_cmd(char* cmd);

typedef struct {
    char* AID;
    char *state;
} AID_state;

typedef struct {
    char* message;
    AID_state state;
}UDP_response;

#endif // UDP_H