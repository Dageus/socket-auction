#ifndef BID_H
#define BID_H

#define BID_CMD         "RBD"
#define BID_NOK_LEN     8
#define BID_ERR_LEN     8
#define BID_ACC_LEN     8

int process_bid(char *input, char **response);

#endif // BID_H