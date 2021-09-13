#include <common.h>
#include <prompt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    init();
    char *cmd_line = NULL;
    int cmd_line_len;
    char *cmd;
    int argc;
    __ssize_t cmd_buffer_size = 0;
    // TODO : Add exit condition
    while (1) {
        prompt();
        
        cmd_line_len = getline(&cmd_line, &cmd_buffer_size, stdin);
        
        argc = 0;
        int argv_size = 16;
        char **argv = (char **)malloc(16 * sizeof(char *)); // Reserve space for 16 args by default

        cmd = strtok(cmd_line, " \t\n");
        argv[argc] = cmd;
        ++argc;
        while (cmd != NULL)
        {
            cmd = strtok(NULL, " \t\n");
            argv[argc] = cmd;
            if ((cmd != NULL) && (++argc >= argv_size)) {
                argv_size *= 2;
                argv = (char **)realloc(argv, argv_size * sizeof(char *));
            }
        }
        cmd = argv[0];
        argv[argc] = NULL;
        
        // Why not free argv elements? Because cmd_line is the same space as argv elements.
        free(argv);
        free(cmd_line);
        cmd_line = NULL;
    }
    return 0;
}