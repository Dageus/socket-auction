#ifndef UNREGISTER_H
#define UNREGISTER_H

#define UNREGISTER_COMMAND "UNR"
#define UNREGISTER_LEN 20

#include "../../constants.h"

int process_unregister(client* user, char** response);

#endif // UNREGISTER_H