#include "exit.h"
#include <stdio.h>
#include <stdlib.h>

int process_exit(int uid) {

    if (uid != -1)
        return -1;
    else
        exit(EXIT_SUCCESS);

    return 0;
}