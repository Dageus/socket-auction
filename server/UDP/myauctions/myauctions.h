#ifndef MYAUCTIONS_H
#define MYAUCTIONS_H

#define MYA_CMD "LMA"

int load_auction(char* aid, auction_list *list_item);
int get_bidded_list(char* uid, auction_list **list);
int process_myauctions(char* input, char** response);

#endif  //MYAUCTIONS_H