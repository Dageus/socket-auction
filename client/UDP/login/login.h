#ifndef LOGIN_H
#define LOGIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LOGIN_CMD "LIN"
#define LOGIN_LEN 20

int process_login(char* input, char** response);

#endif // LOGIN_H