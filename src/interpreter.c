#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../headers/lexer.h"

static token *type_error(lexer *l, toktype expected)
{
    lexer_halt(l);
    char *msg = malloc(MAXBUFSIZE*sizeof(char));
    toktype actual = token_gettype(lexer_peek(l));
    snprintf(msg, MAXBUFSIZE, "Unexpected type %s, expected %s (%d:%d)",
	     tokname(actual), tokname(expected), lexer_getline(l), lexer_getcol(l));
    return token_new(ERR, 0, msg);
}

static token *divisionbyzero(lexer *l)
{
    lexer_halt(l);
    char *msg = malloc(MAXBUFSIZE*sizeof(char));
    snprintf(msg, MAXBUFSIZE, "Division by zero (%d:%d)", lexer_getline(l), lexer_getcol(l));
    return token_new(ERR, 0, msg);
}

static token *expect(lexer *l, toktype expected)
{
    token *next = lexer_next(l);
    toktype actual = token_gettype(next);
    if (actual == expected) {
	return next;
    }
    return NULL;
}

static token *calc(lexer *l, token *op, token *a, token *b)
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
	    if (rh == 0)
		return divisionbyzero(l);
	    result = lh / rh; break;
	default:
	    /* TODO: Error handling - unknown operator */
	    result = 0;
	    break;
    }

    return token_new(NUMBER, result, NULL);
}

token *expr(lexer *l);

token *factor(lexer *l)
{
    token *result;
    token *nexttoken = lexer_peek(l);
    toktype nexttype = token_gettype(nexttoken);
    if (nexttype == LPAREN) {
	token *lp = expect(l, LPAREN);
	token_delete(lp);
	result = expr(l);
	// TODO: Better error handling
	if (token_gettype(result) == ERR) {
	    return result;
	}
	token *rp = expect(l, RPAREN);
	if (!rp) {
	    token_delete(result);
	    return type_error(l, RPAREN);
	}
    }
    else {
	result = expect(l, NUMBER);
	if (!result) {
	    token_delete(result);
	    return type_error(l, NUMBER);
	}
    }
    return result;
}

token *term(lexer *l)
{
    token *result = factor(l);
    if (token_gettype(result) == ERR) {
	return result;
    }
    token *currtok = lexer_peek(l);
    toktype nexttype = token_gettype(currtok);
    while (nexttype == TIMES || nexttype == DIVIDE) {
	token *op, *b;
	if (nexttype == TIMES) {
	    op = expect(l, TIMES);
	    b = term(l);
	    if (token_gettype(b) == ERR) {
		return b;
	    }
	}
	else {
	    op = expect(l, DIVIDE);
	    b = term(l);
	    if (token_gettype(b) == ERR) {
		return b;
	    }
	}
	result = calc(l, op, result, b);
	token_delete(op); token_delete(b);
	currtok = lexer_peek(l);
	nexttype = token_gettype(currtok);
    }
    return result;
}

token *expr(lexer *l)
{
    token *result = term(l);
    if (token_gettype(result) == ERR) {
	return result;
    }
    token *currtok = lexer_peek(l);
    toktype nexttype = token_gettype(currtok);
    while (nexttype == PLUS || nexttype == MINUS) {
	token *op, *b;
	if (nexttype == PLUS) {
	    op = expect(l, PLUS);
	    b = term(l);
	    if (token_gettype(b) == ERR) {
		return b;
	    }
	}
	else {
	    op = expect(l, MINUS);
	    b = term(l);
	    if (token_gettype(b) == ERR) {
		return b;
	    }
	}
	result = calc(l, op, result, b);
	token_delete(op); token_delete(b);
	currtok = lexer_peek(l);
	nexttype = token_gettype(currtok);
    }
    return result;
}
