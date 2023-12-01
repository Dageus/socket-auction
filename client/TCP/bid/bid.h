#ifndef BID_H
#define BID_H

#define BID_LEN 11
#define BID_CMD "BID"

int process_bid(char* input, client** user, char** response);

#endif // BID_H