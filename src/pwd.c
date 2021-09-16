#include <printwd.h>
#include <common.h>

#include <stdio.h>
#include <errno.h>

int pwd(int argc, char **argv) {
    if (argc > 1) {
        errno = E2BIG;
        return 1;
    }

    printf("%s\n", cwdpath);
    return 0;
}