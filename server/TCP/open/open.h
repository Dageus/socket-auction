#ifndef OPEN_H
#define OPEN_H

#define OPEN_RESPONSE       "ROA"
#define OPEN_OK_LEN         7
#define OPEN_ERR_LEN        8
#define OPEN_NOK_LEN        8


#include <stdio.h>

int process_open_auction(int fd, char* input, int aid, char** response);
int create_auction_dir(int aid, char* uid, char* name, char* fname, char* start_value, char* timeactive);
int create_start_file(int aid, char* uid, char* name, char* fname, char* start_value, char* timeactive);

#endif // OPEN_H