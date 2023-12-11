#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <time.h>

#define USERS_DIR       "USERS"
#define AUCTIONS_DIR    "AUCTIONS"

#define OK_STATUS       "OK"
#define NOK_STATUS      "NOK"
#define ERR_STATUS      "ERR"

#define HOSTED          "HOSTED"
#define BIDDED          "BIDDED"

#define BIDS            "BIDS"
#define ASSET           "ASSET"

#define PWD_SUFFIX      "_password.txt"
#define LOGIN_SUFFIX    "_login.txt"
#define TXT_SUFFIX      ".txt"
#define END_PREFIX      "END_"

#define AUCTION_FILE_LEN 7

#define SERVER_IP       "tejo.tecnico.ulisboa.pt"
#define DEFAULT_PORT    "58090"

#define LOCAL_SERVER_IP "localhost"

#define TRUE            1
#define FALSE           0

#define MAX_COMMAND_LEN 56

#define UID_LEN         6
#define PWD_LEN         8

#define MAX_NAME        10

#define MAX_START_LEN   6
#define MAX_DUR_LEN     5
#define MAX_BIDDING_LEN 6

#define AID_LEN         3

#define MAX_FNAME_LEN   24

#define READ_WRITE_RATE 1024

typedef struct {
    char host_uid[UID_LEN + 1];
    char name[MAX_NAME + 1];
    char asset[MAX_FNAME_LEN + 1];
    int start;
    char start_time[20];
    int duration;
    int active;
} auction;

typedef struct {
    char bidder_uid[UID_LEN + 1];
    int bid_value;
    char bid_time[20];
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