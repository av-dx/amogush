#ifndef __LS_H
#define __LS_H

#include <dirent.h>
#include <sys/types.h>

struct efinfo {
    mode_t mode;
    uid_t ownid;
    gid_t grpid;
    nlink_t hardlinks;
    off_t size;
    blkcnt_t blocks;
    time_t mtime;
};

int ls(int argc, char **argv);
int filter_nonhidden(const struct dirent *node_name);
int filter_all(const struct dirent *node_name);
int getfileinfo(const char *path, struct efinfo *info);
void print_file(const char *node_path, const char *node_name, struct efinfo *info, int flag_long);
int sort_file_folders(const void *a, const void *b);

#endif