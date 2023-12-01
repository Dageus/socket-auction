#ifndef LOGIN_H
#define LOGIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LOGIN_CMD "LIN"
#define LOGIN_LEN 21

int process_login(char* input, client** user, char** response);

#endif // LOGIN_H