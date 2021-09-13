#ifndef __PROMPT_H
#define __PROMPT_H

#include <sys/types.h>

static char *hostname;
static char *username;
static char *cwdpath;
static char *cwddisplay;
static char *homepath;
static size_t homepath_len;

void initprompt();
void prompt();

#endif