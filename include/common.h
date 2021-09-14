#ifndef __COMMON_H
#define __COMMON_H

#include <sys/types.h>

extern char *hostname;
extern char *username;
extern char *cwdpath;
extern char *cwddisplay;
extern char *homepath;
extern size_t homepath_len;

typedef int(*cmd_callback)(int, char**);

extern char **builtin_cmds;
extern cmd_callback *builtin_cmd_callbacks;
extern const unsigned int NUM_BUILTIN_CMDS;

enum BuiltinsCMD {
    BUILTIN_CD,
    BUILTIN_LS,
    BUILTIN_ECHO,
    BUILTIN_LIST_START = 0,
};

// TODO: Move init() to main.h
void init();

int builtin_cmd_callback(enum BuiltinsCMD cmdid, int argc, char **argv);

int is_prefix(const char *pfx, const char *str);

#endif