#ifndef MYBIDS_H
#define MYBIDS_H

#include "../../constants.h"

#define MYB_CMD "RMB"

int get_bidded_list(char* uid, auction_list *list);

void process_mybids(char* input, char** response);

#endif  //MYBIDS_H