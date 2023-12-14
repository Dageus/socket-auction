#ifndef CLOSE_H
#define CLOSE_H

#define CLOSE_CMD           "CLS"
#define CLOSE_CMD_OK_LEN    7
#define CLOSE_CMD_END_LEN   8
#define CLOSE_CMD_NLG_LEN   8
#define CLOSE_CMD_ERR_LEN   8
#define CLOSE_CMD_EOW_LEN   8
#define END_STATUS          "END"
#define EOW_STATUS          "EOW"
#define NO_AUCTION          "EAU"
#define NLG_STATUS          "NLG"

int process_close(char *input, char **response);

#endif // CLOSE_H