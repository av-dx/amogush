#include <common.h>
#include <string.h>

char *hostname;
char *username;
char *lwdpath;
char *cwdpath;
char *cwddisplay;
char *homepath;
size_t homepath_len;

int is_prefix(const char *pfx, const char *str) {
    int p_pos = 0;
    size_t pfx_len = strlen(pfx);
    if (strncmp(pfx, str, pfx_len) == 0) {
        return 1;
    }
    return 0;
}