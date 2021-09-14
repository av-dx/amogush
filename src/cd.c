#include <cd.h>
#include <common.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int cd(int argc, char **argv) {
    const char *dest = NULL;
    if (argc > 2) {
        errno = E2BIG;
        return 1;
    } else if (argc < 2) {
        dest = homepath;
    } else {
        dest = argv[1];
    }

    if (chdir(dest) != 0) {
        return 1;
    }

    return 0;
}