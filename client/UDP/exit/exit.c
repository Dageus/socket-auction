#include "exit.h"
#include "../../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int process_exit(client** user) {

    if (strcmp((*user)->uid, NO_UID) == 0) 
        return -1;
    else {
        printf("Exiting client...\n");
        free((*user)->uid);
        free((*user)->pwd);
        free(*user);
        exit(EXIT_SUCCESS);
    }

    return 0;
}