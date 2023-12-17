#ifndef COMMON_H
#define COMMON_H

#define COMMAND_LEN 3

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int get_global_aid_number();
int check_password(char* user_dir, char* uid, char* pwd);
int read_word(int fd, char *buffer, int size);
int check_auction_end(int aid);

#endif // COMMON_H