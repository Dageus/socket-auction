#ifndef SHOW_RECORD_H
#define SHOW_RECORD_H

#define SHOW_RECORD_LEN 11
#define SHOW_RECORD_CMD "SRE"

int process_show_record(char* input, char* uid, char* pwd, char* response);

#endif // SHOW_RECORD_H