#include "global.h"
#include "token.h"

Token new_token(int type, char *val)
{
    Token t;
    t.type = type;
    t.val = val;
    return t;
}

void token_print(Token t)
{
    printf("<type: %d, val: \"%s\" >\n", t.type, t.val);
}

void token_delete(Token t)
{
    free(t.val);
}
