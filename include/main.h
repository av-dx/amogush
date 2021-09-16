#ifndef __MAIN_H
#define __MAIN_H

typedef int(*cmd_callback)(int, char**);

extern char **builtin_cmds;
extern cmd_callback *builtin_cmd_callbacks;
extern const unsigned int NUM_BUILTIN_CMDS;

enum BuiltinsCMD {
    BUILTIN_CD,
    BUILTIN_LS,
    BUILTIN_ECHO,
    BUILTIN_PWD,
    BUILTIN_LIST_START = 0,
};

void init();

void exitshell(int ret);

int builtin_cmd_callback(enum BuiltinsCMD cmdid, int argc, char **argv);

#endif