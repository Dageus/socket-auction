#ifndef LOGOUT_H
#define LOGOUT_H

#define LOGOUT_CMD      "RLO"
#define LOGOUT_OK_LEN   7
#define LOGOUT_NOK_LEN  8
#define LOGOUT_UNR_LEN  8

int check_password(char* user_dir, char* uid, char* pwd);
int process_user_logout(char* input, char** response);

#endif // LOGOUT_H