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
int create_UDP();
int send_UDP(int socket, char *message, struct sockaddr_in *server_addr);

#endif // UDP_H