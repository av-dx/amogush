#ifndef __COMMON_H
#define __COMMON_H

#define CHECK_NULL_FREE(x) if (x != NULL) {free(x);}

#include <sys/types.h>

extern char *hostname;
extern char *username;
extern char *lwdpath;
extern char *cwdpath;
extern char *cwddisplay;
extern char *homepath;
extern size_t homepath_len;

int is_prefix(const char *pfx, const char *str);
void printargs(int argc, char **argv);

#endif