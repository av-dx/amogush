#include <common.h>
#include <string.h>
#include <stdio.h>

char *hostname;
char *username;
char *lwdpath;
char *cwdpath;
char *cwddisplay;
char *homepath;
size_t homepath_len;

int is_prefix(const char *pfx, const char *str) {
    size_t pfx_len = strlen(pfx);
    if (strncmp(pfx, str, pfx_len) == 0) {
        return 1;
    }
    return 0;
}

void printargs(int argc, char **argv) {
    for (int i = 0; i < argc; ++i) {
        printf("Argument %d: %s\n", i, argv[i]);
    }
}