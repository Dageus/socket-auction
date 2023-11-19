#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

int main(){
 // Create a socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        /*error*/
        exit(1);
    }

    return 0;
}