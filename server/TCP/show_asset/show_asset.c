#include "show_asset.h"
#include "../../constants.h"
#include <sys/stat.h>

int CheckAssetFile(char *fname) {
    struct stat filestat;
    int ret_stat;
    ret_stat = stat(fname, &filestat);
    if (ret_stat == -1 || filestat.st_size == 0)
        return 0;
    return filestat.st_size;
}