#ifndef LOGOUT_H
#define LOGOUT_H

#define LOGOUT_COMMAND "LOU"
#define LOGOUT_LEN 11

int process_logout(char *uid, char** response);

#endif // LOGOUT_H