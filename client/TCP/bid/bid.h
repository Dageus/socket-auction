#ifndef BID_H
#define BID_H

#define BID_LEN 11
#define BID_CMD "BID"

#include "../../constants.h"

int process_bid(char* input, client* user, char** response, int *code);

#endif // BID_H