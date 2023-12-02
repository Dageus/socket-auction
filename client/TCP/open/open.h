#ifndef OPEN_H
#define OPEN_H

#define OPEN_LEN 78

#define OPEN_CMD "OPA"

int process_open(char* input, client* user, char** response, int* code);

#endif  // OPEN_H