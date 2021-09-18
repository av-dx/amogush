#define _DEFAULT_SOURCE
#include <ls.h>
#include <common.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

extern char *optarg;
extern int optind, opterr, optopt;

int ls(int argc, char **argv) {
    int opt;
    int flag_long = 0;
    int flag_all = 0;
    optind = 0;
    while ((opt = getopt(argc, argv, "la")) != -1) {
        switch (opt)
        {
        case 'l':
            flag_long = 1;
            break;
        case 'a':
            flag_all = 1;
            break;
        default:
            break;
        }
    }

    struct dirent **filelist;
    int num_files;

    int (*list_filter)(const struct dirent *) = (flag_all) ? NULL : filter_nonhidden;

    if (optind == argc) {
        char *node_name = ".";
        num_files = scandir(node_name, &filelist, list_filter, alphasort);
        if (num_files == -1) {
            return 1;
        }

        for (int k = 0; k < num_files; ++k) {
            printf("%s\n", filelist[k]->d_name);
            free(filelist[k]);
        }
        free(filelist);
    }

    for (int i = optind; i < argc; ++i) {
        char *node_name = argv[i];
        if (argv[i][0] == '~') {
            node_name = malloc(sizeof(*node_name) * 4096);
            strcpy(node_name, homepath);
            strcat(node_name, argv[i] + 1);
        }
        
        num_files = scandir(node_name, &filelist, list_filter, alphasort);
        if (num_files == -1) {
            return 1;
        }

        for (int k = 0; k < num_files; ++k) {
            printf("%s\n", filelist[k]->d_name);
            free(filelist[k]);
        }
        free(filelist);
    }
    
    return 0;
}

int filter_nonhidden(const struct dirent *d) {
    if (d->d_name[0] == '.')
        return 0;
    else return 1;
}