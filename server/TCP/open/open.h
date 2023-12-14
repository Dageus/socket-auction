#ifndef OPEN_H
#define OPEN_H

#define OPEN_RESPONSE "ROA"


#include <stdio.h>

int process_open_auction(int fd, int aid, char** response);
int CreateAUCTIONDir(int aid, char* uid, char* name, char* fname, char* start_value, char* timeactive);
int create_start_file(int aid, char* uid, char* name, char* fname, char* start_value, char* timeactive, char time_str[20], char s_time_str[11]);

#endif // OPEN_H