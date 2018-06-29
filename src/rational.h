#ifndef _RATIONAL_H
#define _RATIONAL_H

typedef struct {
    int numer;
    int denom;
} rational;

rational new_rational(int numer, int denom);

rational rat_add(rational a, rational b);
rational rat_sub(rational a, rational b);
rational rat_mul(rational a, rational b);
rational rat_div(rational a, rational b);

double to_double(rational r);

rational from_str(char *str);
char *rat_str(rational r);

#endif