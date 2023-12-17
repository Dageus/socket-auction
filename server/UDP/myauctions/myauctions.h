#ifndef MYAUCTIONS_H
#define MYAUCTIONS_H

#include "../../constants.h"

#define MYA_CMD "LMA"

int get_hosted_list(char* uid, auction_list *list);
void process_myauctions(char* input, char** response);

#endif  //MYAUCTIONS_H