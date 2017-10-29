#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../headers/lexer.h"

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

token *expr(lexer *l);

token *factor(lexer *l)
{
    token *result;
    token *nexttoken = lexer_peek(l);
    toktype nexttype = token_gettype(nexttoken);
    if (nexttype == LPAREN) {
	token *lp = expect(l, LPAREN);
	result = expr(l);
	token *rp = expect(l, RPAREN);
	token_delete(lp); token_delete(rp);
    }
    else {
	result = expect(l, NUMBER);
    }
    return result;
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
	    /* TODO: Error handling - Division by zero */
	    result = lh / rh; break;
	default:
	    /* TODO: Error handling - unknown operator */
	    result = 0;
	    break;
    }

    return token_new(NUMBER, result, NULL);
}

token *term(lexer *l)
{
    token *result = factor(l);
    token *currtok = lexer_peek(l);
    toktype nexttype = token_gettype(currtok);
    while (nexttype == TIMES || nexttype == DIVIDE) {
	token *op, *b;
	if (nexttype == TIMES) {
	    op = expect(l, TIMES);
	    b = term(l);
	}
	else {
	    op = expect(l, DIVIDE);
	    b = term(l);
	}
	result = calc(op, result, b);
	token_delete(op); token_delete(b);
	currtok = lexer_peek(l);
	nexttype = token_gettype(currtok);
    }
    return result;
}

token *expr(lexer *l)
{
    token *result = term(l);
    token *currtok = lexer_peek(l);
    toktype nexttype = token_gettype(currtok);
    while (nexttype == PLUS || nexttype == MINUS) {
	token *op, *b;
	if (nexttype == PLUS) {
	    op = expect(l, PLUS);
	    b = term(l);
	}
	else {
	    op = expect(l, MINUS);
	    b = term(l);
	}
	result = calc(op, result, b);
	token_delete(op); token_delete(b);
	currtok = lexer_peek(l);
	nexttype = token_gettype(currtok);
    }

    return result;
}
