#ifndef UDP_H
#define UDP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "tejo.tecnico.ulisboa.pt"
#define TEST_PORT 58011
#define DEFAULT_PORT "58090"

#define LOCAL_SERVER_IP "localhost"

int create_UDP();
int send_UDP(int socket, char *message, struct sockaddr_in *server_addr);

#endif // UDP_H