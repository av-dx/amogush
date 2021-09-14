#include <cd.h>
#include <common.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int cd(int argc, char **argv) {
    char *dest = NULL;
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
    } else if (strcmp(dest, "~") == 0) {
        dest = homepath;
    }

    if (chdir(dest) != 0) {
        return 1;
    }
    strcpy(lwdpath, cwdpath);
    getcwd(cwdpath, 4096);

    return 0;
}