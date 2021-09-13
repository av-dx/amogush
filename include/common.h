#ifndef __COMMON_H
#define __COMMON_H

#include <sys/types.h>

extern char *hostname;
extern char *username;
extern char *cwdpath;
extern char *cwddisplay;
extern char *homepath;
extern size_t homepath_len;

extern char **builtin_cmds;
extern const unsigned int NUM_BUILTIN_CMDS;

void init();
int is_prefix(const char *pfx, const char *str);

#endif