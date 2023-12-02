#ifndef CLOSE_H
#define CLOSE_H

#define CLOSE_LEN 25
#define CLOSE_CMD "CLS"

#include "../../constants.h"

int process_close(char *input, client* user, char** response, int* code);

#endif // CLOSE_H