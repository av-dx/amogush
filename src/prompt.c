#include <prompt.h>
#include <common.h>

#include <unistd.h>
#include <string.h>
#include <stdio.h>

void prompt() {
    getcwd(cwdpath, 4096);

    if (is_prefix(homepath, cwdpath)) {
        cwddisplay[0] = '~';
        strcpy(cwddisplay + 1, cwdpath + homepath_len);
    } else {
        strcpy(cwddisplay, cwdpath);
    }

    printf("<%s@%s:%s>", username, hostname, cwddisplay);
}