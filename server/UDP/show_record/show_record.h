#ifndef SHOW_RECORD_H
#define SHOW_RECORD_H

#include "../../constants.h"

int load_auction_info(int aid, auction* auc);
int load_bid(char* pathname, bid_list list_item);
int get_record_list(int aid , bid_list *list);
int process_show_record(char* input, char** response);

#endif // SHOW_RECORD_H