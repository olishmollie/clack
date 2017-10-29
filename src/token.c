#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../headers/token.h"

struct token_t {
    toktype type;
    int value;
    char *err;
};

token *token_new(toktype type, int value, char *err)
{
    token *t = malloc(sizeof(token));
    if (t) {
        t->type = type;
        t->value = value;
        if (err) {
            size_t len = strlen(err) + 1;
            t->err = malloc(len*sizeof(char));
            strncpy(t->err, err, len);
        } else {
            t->err = NULL;
        }
    }
    return t;
}

toktype token_gettype(token* t)
{
    return t->type;
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
    }
}

char *token_tostr(token *t)
{
    char* buf = malloc(150*sizeof(char));
    snprintf(buf, 150, "Token { type: %s, value: %d, err: %s }", tokname(t->type), t->value, t->err);
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
