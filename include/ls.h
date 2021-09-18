#ifndef __LS_H
#define __LS_H

#include <dirent.h>

int ls(int argc, char **argv);
int filter_nonhidden(const struct dirent *);
int filter_all(const struct dirent *);

#endif