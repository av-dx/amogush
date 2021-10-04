#include <pinfo.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

int pinfo(int argc, char **argv) {
    pid_t pid;
    if (argc > 2) {
        errno = E2BIG;
        return 1;
    } else if (argc == 2) {
        pid = atoi(argv[1]);
    } else {
        pid = getpid();
    }
    
    char proc_stat;
    char proc_fgstat = (argc == 1) ? '+' : ' ';
    size_t proc_mem;

    char *pstat_path = malloc(sizeof(char) * 4096);
    sprintf(pstat_path, "/proc/%d/stat", pid);
    FILE *proc = fopen(pstat_path, "r");
    if (proc == NULL) {
        errno = ESRCH;
        return 1;
    }
    fscanf(proc, "%*d %*s ");
    fscanf(proc, "%c ", &proc_stat);
    for (int i = 4; i < 23; ++i) { fscanf(proc, "%*d "); }
    fscanf(proc, "%lu ", &proc_mem);
    
    fclose(proc);
    free(pstat_path);

    char *pexe_symlink = malloc(sizeof(char) * 4096);
    char *pexe_path = malloc(sizeof(char) * 4096);
    sprintf(pexe_symlink, "/proc/%d/exe", pid);
    readlink(pexe_symlink, pexe_path, 4096);

    printf("pid -- %d\n", pid);
    printf("Process Status -- %c%c\n", proc_stat, proc_fgstat);
    printf("memory -- %lu bytes\n", proc_mem);
    printf("Executable Path -- %s\n", pexe_path);

    return 0;
}