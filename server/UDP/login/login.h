#ifndef LOGIN_H
#define LOGIN_H

#define LOGIN_RESPONSE  "RLI"
#define REG_STATUS      "REG"
#define OK_LEN          7
#define NOK_LEN         8
#define REG_LEN         8

int check_password(char* user_dir, char* uid, char* pwd);

int process_user_login(char* input, char** response);

#endif // LOGIN_H