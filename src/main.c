#include <common.h>
#include <main.h>
#include <prompt.h>
#include <echo.h>
#include <cd.h>
#include <printwd.h>
#include <pinfo.h>
#include <ls.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>

char **builtin_cmds;
cmd_callback *builtin_cmd_callbacks;
const unsigned int NUM_BUILTIN_CMDS = 5;

int main() {
    init();

    char    *shell_input;           // Line of input from the shell
    char    *cmd_line;              // Currently being executed command line

    char    *saveptr_cmd_line;      // saveptr used while extracting command lines from input
    char    *saveptr_args;          // saveptr used while extracting arguments from cmd_line

    char    **argv;                 // Array of arguments to the current command line (null-terminated)
    int     argv_size;              // # of arguments space allocated to argument list
    int     argc;                   // # of arguments to the current command line

    char    *arg;                   // Used as a temp variable internally to hold an argument, do not use.
    char    *cmd;                   // The main command in the command line
    int     retval;                 // Value returned from executed command line
    
    argv_size = 16;
    argv = malloc(sizeof(*argv) * argv_size); // Reserve space for 16 args by default

    size_t shell_input_buffer_size = 0;      // Buffer size allocated by getline() to the shell input.

    while (1) {
        prompt();
        
        getline(&shell_input, &shell_input_buffer_size, stdin);

        cmd_line = strtok_r(shell_input, ";\n", &saveptr_cmd_line);
        while (cmd_line != NULL) {
            argc = 0;
            retval = 0;

            arg = strtok_r(cmd_line, " \t\n", &saveptr_args);
            while (arg != NULL) {
                argv[argc] = arg;
                if ((arg != NULL) && (++argc >= argv_size)) {
                    argv_size *= 2;
                    argv = realloc(argv, sizeof(*argv) * argv_size);
                }
                arg = strtok_r(NULL, " \t\n", &saveptr_args);
            }
            argv[argc] = NULL;
            cmd = argv[0];

            if (argc > 0) {
                // TODO: Add assumption that & doesn't allow chaining
                char *lastarg = *(argv + argc - 1);
                int lastarglen = strlen(lastarg);
                if (lastarg[lastarglen - 1] == '&') {
                    printf("Should BG\n");
                    lastarg[lastarglen - 1] = '\0';
                    if (lastarglen <= 1) { --argc; }
                }
            }

            if (argc > 0) {
                if (strcmp(cmd, "exit") == 0) {
                    // Why not free argv elements? Because cmd_line is the same space as argv elements.
                    free(argv);
                    free(shell_input);
                    exitshell(0);
                }

                for (enum BuiltinsCMD i = BUILTIN_LIST_START; i < NUM_BUILTIN_CMDS; ++i) {
                    if (strcmp(cmd, builtin_cmds[i]) == 0) {
                        retval = builtin_cmd_callback(i, argc, argv);
                        if (retval != 0) {
                            perror(builtin_cmds[i]);
                        }
                    }
                }
            }

            cmd_line = strtok_r(NULL, ";\n", &saveptr_cmd_line);
        }
    }
    exit(0);
}

int builtin_cmd_callback(enum BuiltinsCMD cmdid, int argc, char **argv) {
    return builtin_cmd_callbacks[cmdid](argc, argv);
}

void init() {
    // TODO: Add 4096 under assumptions
    lwdpath = malloc(sizeof(*lwdpath) * 4096);
    cwdpath = malloc(sizeof(*cwdpath) * 4096);
    cwddisplay = calloc(4096, sizeof(*cwddisplay));
    homepath = calloc(4096, sizeof(*homepath));

    getcwd(homepath, 4096);
    homepath_len = strlen(homepath);
    strcpy(cwdpath, homepath);
    strcpy(lwdpath, homepath);

    // TODO: Add 4096 under assumptions
    hostname = calloc(64, sizeof(*hostname));
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
        username = calloc(len + 1, sizeof(*username));
        strcpy(username, pw->pw_name);
    } else if (errno) {
        perror("Unable to get user name");
    } else {
        printf("Unable to get user name : Unknown Error\n");
    }

    builtin_cmds = calloc(NUM_BUILTIN_CMDS + 1, sizeof(*builtin_cmds));
    builtin_cmds[BUILTIN_CD] = "cd";
    builtin_cmds[BUILTIN_ECHO] = "echo";
    builtin_cmds[BUILTIN_LS] = "ls";
    builtin_cmds[BUILTIN_PWD] = "pwd";
    builtin_cmds[BUILTIN_PINFO] = "pinfo";

    builtin_cmd_callbacks = malloc(NUM_BUILTIN_CMDS + 1 * sizeof(*builtin_cmd_callbacks));
    builtin_cmd_callbacks[BUILTIN_ECHO] = &echo;
    builtin_cmd_callbacks[BUILTIN_CD] = &cd;
    builtin_cmd_callbacks[BUILTIN_PWD] = &pwd;
    builtin_cmd_callbacks[BUILTIN_LS] = &ls;
    builtin_cmd_callbacks[BUILTIN_PINFO] = &pinfo;
}

void exitshell(int ret) {
    free(cwddisplay);
    cwddisplay = NULL;
    free(cwdpath);
    cwdpath = NULL;
    free(homepath);
    homepath = NULL;
    free(hostname);
    hostname = NULL;
    free(username);
    username = NULL;
    // Why not free builtin_cmds elements? Because double quotes are compile time consts
    free(builtin_cmds);
    builtin_cmds = NULL;
    free(builtin_cmd_callbacks);
    builtin_cmd_callbacks = NULL;
    exit(ret);
}