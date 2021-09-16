#include <ls.h>

#include <stdio.h>
#include <dirent.h>

int ls(int argc, char **argv) {
    DIR *dir;
    struct dirent *entry;
    dir = opendir(".");
    if (dir != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            printf("%s\n", entry->d_name);
        }
        closedir(dir);
    } else {
        perror("");
        return 1;
    }
    
    return 0;
}