#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

#define TRUE 1
#define FALSE 0
#define AS_IP "tejo.tecnico.ulisboa.pt"
#define DEFAULT_PORT 58090

int fd;

int port, verbose;

// initialize variable in case of incomplete command
port = DEFAULT_PORT;
verbose = FALSE;

int validate_args(int argc, char** argv) {

    if (argc == 1) {
        // run AS on localhost
        // no port needed to be specified

        printf("Running AS on localhost\n");
    } else if (argc == 3) {
        // run AS on specified port
        // no port needed to be specified

        if (strcmp(argv[1], "-p") != 0) {
            // error
            // incomplete command, missing -n
            exit(1);
        }

        port = atoi(argv[2]);

        printf("Running AS on specified port: %d\n", port);
    } else if (argc >= 4) {
        // run AS on specified IP
        // run AS on specified port

        if (strcmp(argv[1], "-p") != 0) {
            // error
            // incomplete command, missing -p

            printf("Error: missing -p argument\n");

            exit(1);
        } else if (strcmp(argv[3], "-v") != 0) {
            // error
            // incomplete command, missing -v

            printf("Error: missing -v argument\n");

            exit(1);
        }

        port = atoi(argv[2]);
        verbose = TRUE;

        printf("Running AS on specified IP: %d and specified port: %d\n", ip, port);
    }
}

int main(int argc, char** argv){

    // validate arguments
    validate_args(argc, argv);


    return 0;
}




/**
 * 
 * Estrutura de quando um cliente se liga ao servidor:
 *  - O cliente envia uma mensagem ao servidor
 *  - criar (uid)_pass.txt
 *  - criar (uid)_login.txt
 * 
 * ESTES FICHEIROS SAO TEMPORARIOS E SO EXISTEM ENQUANTO O USER ESTIVER LOGGED IN
 * 
 * SEMPRE QUE O UTILIZADOR VOLTA A ENTRAR È CRIADO UM NOVO FICHEIRO PASS E LOGIN
 * 
 * 
*/