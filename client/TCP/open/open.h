#ifndef OPEN_H
#define OPEN_H

#define OPEN_LEN 78

#define OPEN_CMD "OPA"

#include "../../constants.h"

#include "../TCP.h"

int process_open(char* input, client* user, TCP_response** response);

#endif  // OPEN_H