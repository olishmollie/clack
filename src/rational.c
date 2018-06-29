#include "rational.h"

#include <stdio.h>
#include <stdlib.h>

rational rat_reduce(rational r);

int gcf(int a, int b)
{
    if (b == 0)
        return a;
    return gcf(b, a%b);
}

rational new_rat(int numer, int denom)
{
    rational r;
    int d = gcf(numer, denom);
    r.numer = d < 0 ? -1 * numer/d : numer/d;
    r.denom = d < 0 ? -1 * denom/d : denom/d;
    return r;
}

rational rat_add(rational a, rational b)
{
    int d = a.denom * b.denom;
    return new_rat(a.numer*b.denom + b.numer*a.denom, d);
}

rational rat_sub(rational a, rational b)
{
    int d = a.denom * b.denom;
    return new_rat(a.numer*b.denom - b.numer*a.denom, d);
}

rational rat_mul(rational a, rational b)
{
    return new_rat(a.numer*b.numer, a.denom*b.denom);
}

rational rat_div(rational a, rational b)
{
    return new_rat(a.numer*b.denom, a.denom*b.numer);
}

double to_double(rational r)
{
    return (double)r.numer / (double)r.denom;
}

rational from_str(char *str)
{
    rational r;
    sscanf(str, "%d/%d", &r.numer, &r.denom);
    return r;
}

char *rat_str(rational r)
{
    int len = 0;
    int n = r.numer, d = r.denom;
    len += snprintf(NULL, 0, "%d", r.numer);
    len += snprintf(NULL, 0, "%d", r.denom);
    char *str = malloc(sizeof(char)*len+2);
    sprintf(str, "%d/%d", r.numer, r.denom);
    return str;
}
