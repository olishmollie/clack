#include <stdio.h>
#include <stdlib.h>

#include "../headers/token.h"

struct token_t {
    toktype type;
    char *value;
    char *err;
};

token *token_new(toktype type, char *value, char *err)
{
    token *t = malloc(sizeof(token));
    if (t) {
        t->type = type;
        t->value = value;
        t->err = err;
    }
    return t;
}

toktype token_gettype(token* t)
{
    return t ? t->type : NOOP;
}

char *token_getvalue(token *t)
{
    return t->value;
}

char *token_geterr(token *t)
{
    return t->err;
}

char *tokname(toktype t)
{
    char *names[] = {
        "MAIN", "INT", "FLOAT", "BOOL", "STRING", "PLUS", "MINUS",
        "TIMES", "DIVIDE", "ASSIGN", "IF", "ELSE", "WHILE", "EQUALS",
        "NEQUALS", "LT", "LTE", "GT", "GTE", "LPAREN", "RPAREN", "LBRACE",
        "RBRACE", "QUOTE", "COMMA", "POUND", "IDENT", "ERR", "END",
        "SEMI", "NOOP"
    };
    return names[t];
}

char *token_str(token *t)
{
    char *buf = malloc(MAXBUFSIZE*sizeof(char));
    snprintf(buf, MAXBUFSIZE, "<type: %s, value: '%s', err: '%s'>",
	    tokname(t->type), t->value, t->err);
    return buf;
}

void token_delete(token* t)
{
    if (t) {
        if (t->err)
            free(t->err);
        if (t->value)
            free(t->value);
        free(t);
    }
}

