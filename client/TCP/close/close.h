#ifndef CLOSE_H
#define CLOSE_H

#define CLOSE_LEN 25
#define CLOSE_CMD "CLS"

int process_close(char *input, client** user, char** response);

#endif // CLOSE_H