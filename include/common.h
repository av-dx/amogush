#ifndef __COMMON_H
#define __COMMON_H

#include <sys/types.h>

extern char *hostname;
extern char *username;
extern char *lwdpath;
extern char *cwdpath;
extern char *cwddisplay;
extern char *homepath;
extern size_t homepath_len;

int is_prefix(const char *pfx, const char *str);

#endif