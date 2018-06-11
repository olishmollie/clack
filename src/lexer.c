#include "../headers/global.h"

int linenum = 1;
int tokenval = NONE;
int cur = 0;

int lexan(char *input)
{
    int t;
    while (1) {
        t = input[cur];
        if (t == ' ' || t == '\t')
            cur++;
        else if (t == '\n')
            linenum++;
        else if (isdigit(t)) {
            int k = 0;
            while (isdigit(t)) {
                k = k * 10 + (t - '0');
                t = input[++cur];
            }
            tokenval = k;
            return NUM;
        }
        else if (t == EOF)
            return DONE;
        else {
            tokenval = NONE;
            cur++;
            return t;
        }
    }
}

void reset()
{
    cur = 0;
}