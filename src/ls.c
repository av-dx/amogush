#define _DEFAULT_SOURCE
#include <ls.h>
#include <common.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

extern char *optarg;
extern int optind, opterr, optopt;

int ls(int argc, char **argv) {
    int opt;
    int flag_long = 0;
    int flag_all = 0;
    char **resolved_pathlist = NULL;
    int num_resolved_paths = 0;
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

    struct efinfo **fileinfo;
    struct dirent **filelist;
    int num_files;

    int (*list_filter)(const struct dirent *) = (flag_all) ? NULL : filter_nonhidden;

    if (optind == argc) {
        char *node_name = ".";
        filelist = NULL;
        fileinfo = NULL;
        num_files = scandir(node_name, &filelist, list_filter, alphasort);
        if (num_files == -1) {
            return 1;
        }

        if (num_files != -1) {
            fileinfo = malloc(sizeof(struct efinfo *) * num_files);
            off_t total_blocks = 0;
            for (int k = 0; k < num_files; ++k) {
                fileinfo[k] = malloc(sizeof(struct efinfo));
                char *full_path = malloc(sizeof(char) * 4096);
                strcpy(full_path, node_name);
                strcat(full_path, "/");
                strcat(full_path, filelist[k]->d_name);
                getfileinfo(full_path, fileinfo[k]);
                total_blocks += fileinfo[k]->blocks;
            }
            if (flag_long) { printf("total %ld\n", total_blocks / 2); }
        }
        
        for (int k = 0; k < num_files; ++k) {
            print_file(node_name, filelist[k]->d_name, fileinfo[k], flag_long);
            free(filelist[k]);
            free(fileinfo[k]);
        }
        free(filelist);
        free(fileinfo);
        return 0;
    } else {
        for (int i = optind; i < argc; ++i) {
            if (argv[i][0] == '~') {
                ++num_resolved_paths;
                resolved_pathlist = realloc(resolved_pathlist,
                                           sizeof(*resolved_pathlist) * num_resolved_paths);
                char *resolved_path = calloc(4096, sizeof(*resolved_path));
                resolved_pathlist[num_resolved_paths - 1] = resolved_path;
                strcpy(resolved_path, homepath);
                strcat(resolved_path, argv[i] + 1);
                argv[i] = resolved_path;
            }
        }
        // TODO: Is qsort() mt-safe? man pages say so, but qsort_r() also exists in _GNU_SOURCE...
        qsort(argv + optind, argc - optind, sizeof (*argv), sort_file_folders);
    }

    for (int i = optind; i < argc; ++i) {
        char *node_name = argv[i];
        filelist = NULL;
        fileinfo = NULL;
        num_files = scandir(node_name, &filelist, list_filter, alphasort);
        if (num_files == -1) {
            // TODO : There are other file types than just directory / regular file
            if (errno == ENOTDIR) {
                struct efinfo temp;
                getfileinfo(node_name, &temp);
                print_file(node_name, node_name, &temp, flag_long);
            } else {
                perror("ls");
            }
        }

        if (num_files != -1) {
            if (i != optind) { printf("\n"); }
            if (argc > optind + 1) {printf("%s:\n", node_name);}
            fileinfo = malloc(sizeof(struct efinfo *) * num_files);
            off_t total_blocks = 0;
            for (int k = 0; k < num_files; ++k) {
                fileinfo[k] = malloc(sizeof(struct efinfo));
                char *full_path = malloc(sizeof(char) * 4096);
                strcpy(full_path, node_name);
                strcat(full_path, "/");
                strcat(full_path, filelist[k]->d_name);
                getfileinfo(full_path, fileinfo[k]);
                total_blocks += fileinfo[k]->blocks;
            }
            if (flag_long) { printf("total %ld\n", total_blocks / 2); }
        }
        
        for (int k = 0; k < num_files; ++k) {
            print_file(node_name, filelist[k]->d_name, fileinfo[k], flag_long);
            free(filelist[k]);
            free(fileinfo[k]);
        }
        free(filelist);
        free(fileinfo);
    }
    
    while (num_resolved_paths--) {
        free(resolved_pathlist[num_resolved_paths]);
    }
    free(resolved_pathlist);
    return 0;
}

void print_file(const char *node_path, const char *node_name, struct efinfo *info, int flag_long) {
    if (!flag_long) {
        printf("%s\n", node_name);
        return;
    }

    struct tm *n_mtime = localtime(&(info->mtime));
    char n_mtime_str[16];

    time_t cur_time = time(NULL);
    int recency = ((time_t)difftime(cur_time, info->mtime) / 60) / 60 / 24 / 30;
    // -------------------------------------------s-----------m----h----d----m


    struct passwd *pwo = getpwuid(info->ownid);
    struct group *pwg = getgrgid(info->grpid);

    // TODO : (Maybe) Pretty formatting
    /* Long format
     |---10---|                          |----12----|
     -rwxrwxrwx 1 aashwin aashwin 122353 Feb 24  2020 Filename
    */

    // TODO: Is malloc any faster???
    // TODO: Add assumption about file types displayed
    printf("%c%c%c%c%c%c%c%c%c%c ",(S_ISDIR(info->mode)) ? 'd' : S_ISLNK(info->mode) ? 'l' : '-',
            (S_IRUSR & info->mode) ? 'r' : '-',(S_IWUSR & info->mode) ? 'w' : '-',(S_IXUSR & info->mode) ? 'x' : '-',
            (S_IRGRP & info->mode) ? 'r' : '-',(S_IWGRP & info->mode) ? 'w' : '-',(S_IXGRP & info->mode) ? 'x' : '-',
            (S_IROTH & info->mode) ? 'r' : '-',(S_IWOTH & info->mode) ? 'w' : '-',(S_IXOTH & info->mode) ? 'x' : '-');
    
    printf("%ld ", info->hardlinks);
    printf("%s %s ", pwo->pw_name, pwg->gr_name);
    printf("%ld ", info->size);
    // TODO : Add assumption about 6 months
    if (recency < 6) {
        strftime(n_mtime_str, 13, "%b %d %H:%M", n_mtime);
    } else {
        strftime(n_mtime_str, 13, "%b %d %Y", n_mtime);
    }
    printf("%s ", n_mtime_str);
    printf("%s", node_name);
    if (S_ISLNK(info->mode)) {
        char *absolute_path = calloc(4096, sizeof(char));

        if (node_name != node_path) {
            char *full_path = malloc(sizeof(*full_path) * 4096);
            strcpy(full_path, node_path);
            strcat(full_path, "/");
            strcat(full_path, node_name);
            readlink(full_path, absolute_path, 4096);
            free(full_path);
        } else {
            readlink(node_path, absolute_path, 4096);
        }

        printf(" -> %s\n", absolute_path);
        free(absolute_path);
    } else {
        printf("\n");
    }
}

int filter_nonhidden(const struct dirent *d) {
    if (d->d_name[0] == '.')
        return 0;
    else return 1;
}

int filter_all(const struct dirent *d) {
    return 1;
}

int sort_file_folders(const void *a, const void *b) {
    char *a_name = *((char **)a);
    char *b_name = *((char **)b);
    int order = 0;

    // TODO : stat-ing every time is Ineffecient :(
    struct stat a_stat, b_stat;
    if ((stat(a_name, &a_stat) == -1) || (stat(b_name, &b_stat) == -1)) {
        // Do not raise an error here, let ls() catch it... ?
        return 0;
    } else {
        if (S_ISDIR(a_stat.st_mode)) {
            if (S_ISDIR(b_stat.st_mode)) {
                // Both are directories
                order = strcmp(a_name, b_name);
            } else {
                // a is dir but not b
                order = 1;
            }
        } else {
            if (S_ISDIR(b_stat.st_mode)) {
                // b is dir but not a
                order = -1;
            } else {
                // Both are files
                order = strcmp(a_name, b_name);
            }
        }
        return order;
    }

    return 0;
}

int getfileinfo(const char *path, struct efinfo *info) {
    struct stat node_stat;
    lstat(path, &node_stat);

    info->mode = node_stat.st_mode;
    info->hardlinks = node_stat.st_nlink;
    info->ownid = node_stat.st_uid;
    info->grpid = node_stat.st_gid;
    info->size = node_stat.st_size;
    info->mtime = node_stat.st_mtim.tv_sec;
    info->blocks = node_stat.st_blocks;

    return 0;
}