#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include "commands.h"

#define TRUE 1

#define SERVER_IP "tejo.tecnico.ulisboa.pt"
#define TEST_PORT 58011
#define DEFAULT_PORT 58090

#define SERVER_IP "localhost"

struct addrinfo
{
  int ai_flags;			/* Input flags.  */
  int ai_family;		/* Protocol family for socket.  */
  int ai_socktype;		/* Socket type.  */
  int ai_protocol;		/* Protocol for socket.  */
  socklen_t ai_addrlen;		/* Length of socket address.  */
  struct sockaddr *ai_addr;	/* Socket address for socket.  */
  char *ai_canonname;		/* Canonical name for service location.  */
  struct addrinfo *ai_next;	/* Pointer to next in list.  */
};

int fd,errcode;
ssize_t n;
socklen_t addrlen;
struct addrinfo hints, *res;
struct sockaddr_in addr;
char buffer[128];

// initialize variables
int fd, n;

int ip, port;

// initialize default values in case of incomplete command

ip = SERVER_IP;
port = DEFAULT_PORT;

int validate_args(int argc, char** argv) {

    if (argc == 1) {
        // run AS on localhost
        // no port needed to be specified

        printf("Running AS on localhost\n");
    } else if (argc == 3) {
        // run AS on specified IP
        // no port needed to be specified

        if (strcmp(argv[1], "-n") != 0) {
            // error
            // incomplete command, missing -n
            exit(1);
        }

        ip = atoi(argv[2]);

        printf("Running AS on specified IP: %d\n", ip);
    } else if (argc >= 5) {
        // run AS on specified IP
        // run AS on specified port

        if (strcmp(argv[1], "-n") != 0) {
            // error
            // incomplete command, missing -n

            printf("Error: missing -n argument\n");

            exit(1);
        } else if (strcmp(argv[3], "-p") != 0) {
            // error
            // incomplete command, missing -p

            printf("Error: missing -p argument\n");

            exit(1);
        }

        ip = atoi(argv[2]);
        port = atoi(argv[4]);

        printf("Running AS on specified IP: %d and specified port: %d\n", ip, port);
    }
}

void process_cmd(char* input){
    if (strcmp(input, "login") == 0) {
        process_login();
    } else if (strcmp(input, "logout") == 0) {
        process_logout();
    } else if (strcmp(input, "unregister") == 0) {
        process_unregister();
    } else if (strcmp(input, "exit") == 0) {
        process_exit();
    } else if (strcmp(input, "open") == 0) {
        process_open();
    } else if (strcmp(input, "close") == 0) {
        process_close();
    } else if (strcmp(input, "myauctions") == 0 || strcmp(input, "ma") == 0) {
        process_auctions();
    } else if (strcmp(input, "mybids") == 0 || strcmp(input, "mb") == 0) {
        process_bids();
    } else if (strcmp(input, "list") == 0 || strcmp(input, "l") == 0) {
        process_list();
    } else if (strcmp(input, "show_asset") == 0 || strcmp(input, "sa") == 0) {
        process_asset();
    } else if (strcmp(input, "bid") == 0 || strcmp(input, "b") == 0) {
        process_bid();
    } else if (strcmp(input, "show_record") == 0 || strcmp(input, "sr") == 0) {
        process_record();
        
    } else {
        fprintf(stderr, "error: unkown_command\n");
    }
}

int main(int argc, char** argv) {

    // check if command is complete

    validate_args(argc, argv);


    // Create a socket
    fd = socket(AF_INET,SOCK_DGRAM,0);
    if (fd == -1) {
        /*error*/
        exit(1);
    }

    int tries = 0;

    // main code
    while (TRUE) {

        char* input[MAX_COMMAND_LEN];

        // read from terminal
        scanf("%s", input);

        // see which command was inputted
        process_cmd(input);
    }

    return 0;
}