#include <echo.h>
#include <stdio.h>
#include <string.h>

int echo(int argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
        printf("%s", argv[i]);
        if (i < argc - 1)
            printf(" ");
    }
    printf("\n");
    return 0;
}