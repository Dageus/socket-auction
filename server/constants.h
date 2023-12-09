#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <time.h>

time_t fulltime;
struct tm* currenttime;
char time_str[20];

#define USERS_DIR       "USERS"
#define AUCTIONS_DIR    "AUCTIONS"

#define HOSTED          "HOSTED"
#define BIDDED          "BIDDED"

#define BIDS            "BIDS"
#define ASSET           "ASSET"

#define PWD_SUFFIX "_password.txt"
#define LOGIN_SUFFIX "_login.txt"
#define TXT_SUFFIX ".txt"
#define END_PREFIX "END_"

#define AUCTION_FILE_LEN 7

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

#define TRANSMISSION_RATE 1024

typedef struct {
    char host_uid[UID_LENGTH + 1];
    char name[MAX_NAME + 1];
    char asset[MAX_FILENAME_LEN + 1];
    int start;
    struct tm* start_time;
    int duration;
    int active;
} auction;

typedef struct {
    char bidder_uid[UID_LENGTH + 1];
    int bid_value;
    struct tm* bid_time;
    int bid_sec_tme;
    int active;
} bid_list;

typedef struct {
    char auction_code[4];
    int active;
} auction_list;

typedef struct client {
    char* uid;
    char* pwd;
} client;

#endif // CONSTANTS_H