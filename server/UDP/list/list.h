#ifndef LIST_H
#define LIST_H

#define LIST_CMD        "RLS"
#define LIST_NOK_LEN    8
#define LIST_ERR_LEN    8
#define LIST_OK_LEN     5003

int process_list( char** response);

#endif // LIST_H