#include "headers/global.h"

void error(char *m)
{
    fprintf(stderr, "line %d: %s\n", linenum, m);
    exit(1);
}