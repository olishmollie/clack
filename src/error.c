#include "global.h"
#include "evaluator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fatal(char *m)
{
    stack_clear();
    fprintf(stderr, "%s\n", m);
    exit(1);
}