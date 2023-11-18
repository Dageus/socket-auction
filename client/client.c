#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

#define SERVER_IP "tejo.tecnico.ulisboa.pt"
#define TEST_PORT 58011
#define DEFAULT_PORT 58090

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

int main(int argc, char** argv) {

    // check if command is complete

    int ip, port;

    // initialize default values in case of incomplete command

    ip = SERVER_IP;
    port = DEFAULT_PORT;

    if (argc == 1) {
        // run AS on localhost
        // no port needed to be specified

        printf("Running AS on localhost\n");


    }

    else if (argc == 3) {
        // run AS on specified IP
        // no port needed to be specified

        if (strcmp(argv[1], "-n") != 0) {
            // error
            // incomplete command, missing -n
            exit(1);
        }

        ip = atoi(argv[2]);

        printf("Running AS on specified IP: %d\n", ip);

        // connect to server on specified IP via socket
        // then listen to the terminal for commands

        // code here ---------
    }
    else if (argc >= 5) {
        // run AS on specified IP
        // run AS on specified port

        if (strcmp(argv[1], "-n") != 0) {
            // error
            // incomplete command, missing -n

            printf("Error: missing -n argument\n");

            exit(1);
        }
        else if (strcmp(argv[3], "-p") != 0) {
            // error
            // incomplete command, missing -p

            printf("Error: missing -p argument\n");

            exit(1);
        }

        ip = atoi(argv[2]);
        port = atoi(argv[4]);

        printf("Running AS on specified IP: %d and specified port: %d\n", ip, port);

        // connect to server on specified IP and port via socket
        // then listen to the terminal for commands

        // code here ---------
    }

    // initialize variables
    int fd, n;

    // Create a socket
    fd = socket(AF_INET,SOCK_DGRAM,0);
    if (fd==-1) {
        /*error*/
        exit(1);
    }

    memset(&hints, 0, sizeof(hints));
    
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_DGRAM;

    //UDP socket
    //IPv4
    //UDP socket

    errcode = getaddrinfo("tejo.tecnico.ulisboa.pt",port, &hints, &res);
    if(errcode!=0){
        /*error*/ 
        exit(1);
    }

    addrlen = sizeof(addr);
    n = recvfrom(fd,buffer ,128, 0, (struct sockaddr*)&addr,&addrlen);
    if (n == -1) {
        /*error*/
        exit(1);
    }

    write(1,"echo: ",6); 
    
    write(1,buffer,n);

    freeaddrinfo(res);

    close(fd);

    return 0;
}