#ifndef UDP_H
#define UDP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "logout/logout.h"
#include "login/login.h"
#include "unregister/unregister.h"
#include "list/list.h"
#include "exit/exit.h"
#include "show_record/show_record.h"
#include "myauctions/myauctions.h"
#include "mybids/mybids.h"

int UDP_cmd(char* cmd);
int send_UDP(char *msg, char** udp_buffer, char* ip, char* port);

#endif // UDP_H