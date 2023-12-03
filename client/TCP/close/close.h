#ifndef CLOSE_H
#define CLOSE_H

#define CLOSE_LEN 25
#define CLOSE_CMD "CLS"

#include "../TCP.h"
#include "../../constants.h"

int process_close(char *input, client* user, TCP_response** response);

#endif // CLOSE_H