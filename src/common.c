#include <common.h>
#include <string.h>

char *hostname = NULL;
char *username = NULL;
char *cwdpath = NULL;
char *cwddisplay = NULL;
char *homepath = NULL;
size_t homepath_len = 0;

int is_prefix(const char *pfx, const char *str) {
    int p_pos = 0;
    size_t pfx_len = strlen(pfx);
    if (strncmp(pfx, str, pfx_len) == 0) {
        return 1;
    }
    return 0;
}