#ifndef SHOW_ASSET_H
#define SHOW_ASSET_H

#define SHOW_ASSET_CMD     "RSA"
#define SHOW_ASSET_NOK_LEN 8
#define SHOW_ASSET_ERR_LEN 8

int process_show_asset(char* input, int fd);

#endif // SHOW_ASSET_H