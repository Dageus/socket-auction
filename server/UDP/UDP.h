#ifndef UDP_H
#define UDP_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../constants.h"
#include "list/list.h"
#include "show_record/show_record.h"
#include "login/login.h"
#include "logout/logout.h"
#include "unregister/unregister.h"
#include "myauctions/myauctions.h"
#include "mybids/mybids.h"

void load_auction(char* aid, auction_list* list_item);

int UDP_cmd(char* cmd);

#endif // UDP_H