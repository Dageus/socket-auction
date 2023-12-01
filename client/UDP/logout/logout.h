#ifndef LOGOUT_H
#define LOGOUT_H

#define LOGOUT_COMMAND "LOU"
#define LOGOUT_LEN 20

int process_logout(client** user, char** response);

#endif // LOGOUT_H