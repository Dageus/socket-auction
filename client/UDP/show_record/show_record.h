#ifndef SHOW_RECORD_H
#define SHOW_RECORD_H

#define SHOW_RECORD_COMMAND "SRC"
#define SHOW_RECORD_LEN 8

#include "../../constants.h"

int process_show_record(client* user, char* input, char** response);

#endif // SHOW_RECORD_H