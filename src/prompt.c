#include <prompt.h>
#include <common.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <string.h>

void initprompt() {
    hostname = NULL;
    username = NULL;
    cwdpath = NULL;
    cwddisplay = (char *)calloc(4096, sizeof(char));
    homepath = (char *)calloc(4096, sizeof(char));

    getcwd(homepath, 4096);
    homepath_len = strlen(homepath);
}

void prompt() {
    if (hostname == NULL) {
        hostname = (char *)calloc(64, sizeof(char));
        if (gethostname(hostname, 64) == -1) {
            perror("Unable to get host name");
        } else {
            hostname[63] = '\0';
        }
    }

    if (username == NULL) {
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
    }
    
    if (cwdpath == NULL) {
        // TODO: Add 4096 under assumptions
        cwdpath = (char *)malloc(4096 * sizeof(char));
    }
    getcwd(cwdpath, 4096);

    if (is_prefix(homepath, cwdpath)) {
        cwddisplay[0] = '~';
        strcpy(cwddisplay + 1, cwdpath + homepath_len);
    }

    printf("<%s@%s:%s>", username, hostname, cwddisplay);
}