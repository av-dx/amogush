#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <common.h>
#include <prompt.h>

int main() {
    initprompt();
    char *cmd = NULL;
    int cmd_line_size = 0;
    __ssize_t cmd_buffer_size = 0;
    // TODO : Add exit condition
    while (1) {
        prompt();
        cmd_line_size = getline(&cmd, &cmd_buffer_size, stdin);
    }
    return 0;
}