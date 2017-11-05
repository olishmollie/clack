#include <stdio.h>
#include <stdlib.h>

#include "../headers/token.h"

struct token_t {
    toktype type;
    int value;
    char *name;
    char *err;
};

token *token_new(toktype type, int value, char *name, char *err)
{
    token *t = malloc(sizeof(token));
    if (t) {
        t->type = type;
        t->value = value;
	t->name = name;
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

void token_setvalue(token* t, int val)
{
    t->value = val;
}

char *token_getname(token *t)
{
    return t->name;
}

char *tokname(toktype t)
{
    switch (t) {
        case NUMBER: return "NUMBER";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case TIMES: return "TIMES";
        case DIVIDE: return "DIVIDE";
	case EQUALS: return "EQUALS";
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
	    tokname(t->type), t->value, t->name, t->err);
    return buf;
}

void token_delete(token* t)
{
    if (t) {
        if (t->err) {
            free(t->err);
        }
        if (t->name) {
            free(t->name);
        }
	free(t);
    }
}

