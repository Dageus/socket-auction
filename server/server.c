#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

int main(){


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