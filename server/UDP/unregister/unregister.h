#ifndef UNREGISTER_H
#define UNREGISTER_H

#define RUR_CMD "RUR"
#define UNR_CMD "UNR"

#define UNREGISTER_OK_LEN 7
#define UNREGISTER_NOK_LEN 8
#define UNREGISTER_UNR_LEN 8

void process_unregister(char* input, char** response);

#endif //UNREGISTER_H