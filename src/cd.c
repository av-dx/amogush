#include <cd.h>
#include <common.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int cd(int argc, char **argv) {
    char *dest = NULL;
    char *resolvedpath = NULL;
    if (argc > 2) {
        errno = E2BIG;
        return 1;
    } else if (argc < 2) {
        dest = homepath;
    } else {
        dest = argv[1];
    }

    if (strcmp(dest, "-") == 0) {
        dest = lwdpath;
    } else if (dest[0] == '~') {
        // TODO : Consider the case when homepath + argv[1].size > 4096
        resolvedpath = (char *)malloc(4096 * sizeof(char));
        strcpy(resolvedpath, homepath);
        strcat(resolvedpath, dest + 1);
        dest = resolvedpath;
    }

    if (chdir(dest) != 0) {
        return 1;
    }
    strcpy(lwdpath, cwdpath);
    getcwd(cwdpath, 4096);

    if (resolvedpath != NULL) { free(resolvedpath); }
    return 0;
}