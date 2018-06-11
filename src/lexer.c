#include "../headers/global.h"

int linenum = 1;
int tokenval = NONE;

int lexan() {
    int t;
    while (1) {
        t = getchar();
        if (t == ' ' || t == '\t')
            ;
        else if (t == '\n')
            linenum++;
        else if (isdigit(t)) {
            ungetc(t, stdin);
            scanf("%d", &tokenval);
            return NUM;
        }
        else if (t == EOF)
            return DONE;
        else {
            tokenval = NONE;
            return t;
        }
    }
}