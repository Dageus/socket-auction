#ifndef SHOW_ASSET_H
#define SHOW_ASSET_H

#define SHOW_ASSET_LEN 11

#include "../TCP.h"

int process_show_asset(char* input, TCP_response** response);

#endif // SHOW_ASSET_H