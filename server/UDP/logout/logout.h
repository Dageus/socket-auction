#ifndef LOGOUT_H
#define LOGOUT_H

int check_password(char* user_dir, char* uid, char* pwd);
int process_user_logout(char* input, char** response);

#endif // LOGOUT_H