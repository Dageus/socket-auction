#ifndef BID_H
#define BID_H

#define BID_LEN 11
#define BID_CMD "BID"

#include "../../constants.h"
#include "../TCP.h"

int process_bid(char* input, client* user, TCP_response** response);

#endif // BID_H