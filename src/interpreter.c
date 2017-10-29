#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../headers/lexer.h"

// TODO: clean up macros
#define MAXBUFSIZE 150

static token *throw(lexer *l, toktype actual, toktype expected)
{
    lexer_halt(l);
    char *msg = malloc(MAXBUFSIZE*sizeof(char));
    snprintf(msg, MAXBUFSIZE, "Unexpected type %s, expected %s (%d:%d)",
	     tokname(actual), tokname(expected), lexer_getline(l), lexer_getcol(l));
    return token_new(ERR, 0, msg);
}

static token *expect(lexer *l, toktype expected)
{
    token *next = lexer_next(l);
    toktype actual = token_gettype(next);
    if (actual == expected) {
	return next;
    }
    return throw(l, actual, expected);
}

token *interpreter_factor(lexer *l)
{
    return expect(l, NUMBER);
}

token *interpreter_term(lexer *l)
{
    return expect(l, NUMBER);
}

static token *calc(token *op, token *a, token *b)
{
    int result;
    int lh = token_getvalue(a);
    int rh = token_getvalue(b);

    switch (token_gettype(op)) {
	case PLUS:
	    result = lh + rh; break;
	case MINUS:
	    result = lh - rh; break;
	case TIMES:
	    result = lh * rh; break;
	case DIVIDE:
	    // TODO: Error handling - Division by zero
	    result = lh / rh; break;
	default:
	    //TODO: Error handling - unknown operator
	    result = 0;
	    break;
    }

    return token_new(NUMBER, result, NULL);
}

token *interpreter_expr(lexer *l)
{
    token *result;
    token *a = interpreter_term(l);
    token *op = lexer_next(l);
    token *b = interpreter_term(l);

    result = calc(op, a, b);
    token_delete(a);
    token_delete(op);
    token_delete(b);

    return result;
}
