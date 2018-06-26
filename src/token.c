#include "token.h"
#include "error.h"

#include <string.h>

Token token_new(int type, char *val, int len)
{
    Token t;
    t.type = type;
    if (len >= MAXLEN)
        fatal("token value too long");
    strncpy(t.val, val, len+1);
    t.val[len] = '\0';
    return t;
}

void token_print(Token t)
{
    printf("<type: %d, val: \"%s\" >\n", t.type, t.val);
}
