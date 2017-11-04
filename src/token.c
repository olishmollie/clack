#include <stdio.h>
#include <stdlib.h>

#include "../headers/token.h"

struct token_t {
    toktype type;
    int value;
    char *str;
    char *err;
};

token *token_new(toktype type, int value, char *str, char *err)
{
    token *t = malloc(sizeof(token));
    if (t) {
        t->type = type;
        t->value = value;
	t->str = str;
	t->err = err;
    }
    return t;
}

toktype token_gettype(token* t)
{
    return t ? t->type : NIL;
}

int token_getvalue(token *t)
{
    return t->value;
}

char *tokname(toktype t)
{
    switch (t) {
        case NUMBER: return "NUMBER";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case TIMES: return "TIMES";
        case DIVIDE: return "DIVIDE";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case IDENT: return "IDENT";
        case ERR: return "ERR";
        case END: return "EOF";
	case NIL: return "NIL";
    }
}

char *token_str(token *t)
{
    char *buf = malloc(MAXBUFSIZE*sizeof(char));
    snprintf(buf, MAXBUFSIZE, "<type: %s, value: %d, str: '%s', err: '%s'>",
	    tokname(t->type), t->value, t->str, t->err);
    return buf;
}

void token_delete(token* t)
{
    if (t) {
        if (t->err) {
	    free(t->err);
        }
	free(t);
    }
}

