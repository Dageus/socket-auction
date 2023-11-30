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

// struct addrinfo
// {
//   int ai_flags;			/* Input flags.  */
//   int ai_family;		/* Protocol family for socket.  */
//   int ai_socktype;		/* Socket type.  */
//   int ai_protocol;		/* Protocol for socket.  */
//   socklen_t ai_addrlen;		/* Length of socket address.  */
//   struct sockaddr *ai_addr;	/* Socket address for socket.  */
//   char *ai_canonname;		/* Canonical name for service location.  */
//   struct addrinfo *ai_next;	/* Pointer to next in list.  */
// };

int errcode;
ssize_t n;
socklen_t addrlen;
struct addrinfo hints, *res;
struct sockaddr_in addr;
char buffer[128];

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

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    errcode = getaddrinfo(SERVER_IP, TEST_PORT, &hints, &res);

    n = sendto(fd, msg, strlen(msg), 0, res->ai_addr, res->ai_addrlen);
	if (n == -1) {
		/*error*/
		fprintf(stderr, "Error sending message to server\n");
        return -1;
	}
       
    addrlen = sizeof(addr);
    n = recvfrom(fd, buffer, 128, 0, (struct sockaddr*)&addr, &addrlen);

    printf("Received from server: %s", buffer);

	freeaddrinfo(res);
    close(fd);

    return 1;
}
