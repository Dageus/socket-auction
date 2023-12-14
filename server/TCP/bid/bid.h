#ifndef BID_H
#define BID_H

#define BID_CMD         "RBD"
#define BID_NOK_LEN     8
#define BID_ERR_LEN     8
#define BID_OK_LEN      7

int process_bid(char *input, char **response);

#endif // BID_H