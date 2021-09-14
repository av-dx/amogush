#include <common.h>
#include <echo.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <string.h>

char *hostname;
char *username;
char *cwdpath;
char *cwddisplay;
char *homepath;
size_t homepath_len;

char **builtin_cmds;
cmd_callback *builtin_cmd_callbacks;
const unsigned int NUM_BUILTIN_CMDS = 3;

void init() {
    // TODO: Add 4096 under assumptions
    cwdpath = (char *)malloc(4096 * sizeof(char));
    cwddisplay = (char *)calloc(4096, sizeof(char));
    homepath = (char *)calloc(4096, sizeof(char));

    getcwd(homepath, 4096);
    homepath_len = strlen(homepath);

    // TODO: Add 4096 under assumptions
    hostname = (char *)calloc(64, sizeof(char));
    if (gethostname(hostname, 64) == -1) {
        perror("Unable to get host name");
    } else {
        hostname[63] = '\0';
    }

    errno = 0;
    __uid_t userid = getuid();
    struct passwd *pw = getpwuid(userid);
    if (pw != NULL) {
        int len = strlen(pw->pw_name);
        username = (char *)calloc(len + 1, sizeof(char));
        strcpy(username, pw->pw_name);
    } else if (errno) {
        perror("Unable to get user name");
    } else {
        printf("Unable to get user name : Unknown Error\n");
    }

    builtin_cmds = (char **)calloc(NUM_BUILTIN_CMDS + 1, sizeof(char *));
    builtin_cmds[BUILTIN_CD] = "cd";
    builtin_cmds[BUILTIN_ECHO] = "echo";
    builtin_cmds[BUILTIN_LS] = "ls";

    builtin_cmd_callbacks = malloc(NUM_BUILTIN_CMDS + 1 * sizeof(cmd_callback));
    builtin_cmd_callbacks[BUILTIN_ECHO] = &echo;
}

int builtin_cmd_callback(enum BuiltinsCMD cmdid, int argc, char **argv) {
    return builtin_cmd_callbacks[cmdid](argc, argv);
}

int is_prefix(const char *pfx, const char *str) {
    int p_pos = 0;
    size_t pfx_len = strlen(pfx);
    if (strncmp(pfx, str, pfx_len) == 0) {
        return 1;
    }
    return 0;
}