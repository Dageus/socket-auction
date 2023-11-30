#ifndef UNREGISTER_H
#define UNREGISTER_H

#define UNREGISTER_COMMAND "UNR"
#define UNREGISTER_LEN 20

int process_unregister(char* uid, char* pwd, char** response);

#endif // UNREGISTER_H