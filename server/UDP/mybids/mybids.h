#ifndef MYBIDS_H
#define MYBIDS_H

#include "../../constants.h"

#define MYB_CMD "LMB"

int load_auction(char* aid, auction_list list_item);

int get_bidded_list(char* uid, auction_list *list);

int process_mybids(char* input, char** response);

#endif  //MYBIDS_H