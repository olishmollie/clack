#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fatal(char *m)
{
    fprintf(stderr, "%s\n", m);
    exit(1);
}