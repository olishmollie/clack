#include <stdlib.h>
#include <stdio.h>

#include "../headers/token.h"

struct token_t {
    toktype type;
    int value;
};

token *token_new(toktype type, int value)
{
    token *t = malloc(sizeof(token));
    if (t) {
	t->type = type;
	t->value = value;
    }
    return t;
}

toktype token_gettype(token* t)
{
    return t->type;
}

char* token_tostr(token *t)
{
    char* buf = malloc(150*sizeof(char));
    snprintf(buf, 150, "Token { type: %d, value: %d }", t->type, t->value);
    return buf;
}

void token_delete(token* t)
{
    if (t)
	free(t);
}
