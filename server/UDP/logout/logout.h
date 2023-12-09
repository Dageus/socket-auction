#ifndef LOGOUT_H
#define LOGOUT_H

int check_password(char *username, char *password);
int process_user_logout(char* user_dir, char* uid, char* pwd);

#endif // LOGOUT_H