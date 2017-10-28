#include <stdlib.h>

#include "token.h"

enum toktype {
    NUMBER, PLUS, MINUS, MULT, DIV,
    LPAREN, RPAREN, END
};

struct token_t {
    toktype type;
    void* value;
};

token *token_new(toktype type)
{
    token *t = malloc(sizeof(token));
    if (t) {
	t->type = type;
	t->value = NULL;
    }
    return t;
}

void token_setvalue(token *t, void* value)
{
    if (t)
	t->value = value;
}
