#include "UDP.h"
#include "../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int udp_errcode;
ssize_t udp_n;
socklen_t udp_addrlen;
struct addrinfo udp_hints, *udp_res;
struct sockaddr_in udp_addr;
char udp_buffer[TRANSMISSION_RATE];

const char* UDP_commands[] = {
    "login",
    "logout",
    "unregister",
    "myauctions",
    "ma",
    "mybids",
    "mb",
    "list",
    "l",
    "show_record",
    "sr"
};

int UDP_cmd(char* cmd){
    int i;
    for (i = 0; i < 11; i++) {
        if (strcmp(cmd, UDP_commands[i]) == 0) {
            return 1;
        }
    }
    return -1;
}

int send_UDP(char* msg) {

	int fd = socket(AF_INET,SOCK_DGRAM, 0);
    if (fd == -1) {
        /*error*/
		fprintf(stderr, "Error creating socket\n");
		return -1;
    }

    memset(&udp_hints, 0, sizeof(udp_hints));
    udp_hints.ai_family = AF_INET;
    udp_hints.ai_socktype = SOCK_DGRAM;

    udp_errcode = getaddrinfo(SERVER_IP, TEST_PORT, &udp_hints, &udp_res);

    udp_n = sendto(fd, msg, strlen(msg), 0, udp_res->ai_addr, udp_res->ai_addrlen);
	if (udp_n == -1) {
		/*error*/
		fprintf(stderr, "Error sending message to server\n");
        return -1;
	}
       
    udp_addrlen = sizeof(udp_addr);
    udp_n = recvfrom(fd, udp_buffer, 128, 0, (struct sockaddr*)&udp_addr, &udp_addrlen);

    printf("Received from server: %s", udp_buffer);

	freeaddrinfo(udp_res);
    close(fd);

    return 1;
}
