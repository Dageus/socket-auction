#ifndef CONSTANTS_H
#define CONSTANTS_H

typedef struct client {
    char* uid;
    char* pwd;
} client;

#define SERVER_IP "tejo.tecnico.ulisboa.pt"
#define TEST_PORT "58011"
#define DEFAULT_PORT "58090"

#define LOCAL_SERVER_IP "localhost"

#define TRUE 1
#define FALSE 0

#define MAX_COMMAND_LEN 56

#define UID_LENGTH 6
#define PASSWORD_LEN 8

#define NO_UID "000000"
#define NO_PWD "00000000"

#define MAX_NAME 10

#define MAX_START_ORDER 6
#define MAX_DURATION_ORDER 5
#define MAX_BIDDING_LEN 11

#define AID_LEN 3

#define MAX_FILENAME_LEN 24

#define TRANSMISSION_RATE 512

#endif // CONSTANTS_H