#include "global.h"

int stack[MAXSIZE];
int ip = -1;

int stackpeek()
{
    return stack[ip];
}

void stackset(int k)
{
    stack[ip] = k;
}

void stackpush(int k)
{
    stack[++ip] = k;
}

int stackpop()
{
    if (ip > 0) return stack[ip--];
    else error("cannot stackpop from empty stack");
    return 0;
}
