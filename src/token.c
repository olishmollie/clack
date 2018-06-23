#include "token.h"

Token NewToken(int type, char *val)
{
    Token t;
    t.type = type;
    t.val = val;
    return t;
}

void TokenPrint(Token t)
{
    printf("<type: %d, val: %s >\n", t.type, t.val);
}

void TokenDelete(Token t)
{
    free(t.val);
}
