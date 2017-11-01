#include <stdlib.h>

#include "../headers/lexer.h"

void type_error(lexer *l, toktype unexpected)
{
    char *msg = malloc(MAXBUFSIZE*sizeof(char));
    token_new(ERR, -1, NULL, msg);
}

static void expect(lexer *l, toktype expected)
{
    token *curr = lexer_peek(l);
    toktype actual = token_gettype(curr);
    (actual == expected) ? lexer_next(l) : type_error(l, actual);
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
	    /* TODO: Error handling - division by zero */
	    result = lh / rh; break;
	default:
	    /* TODO: Error handling - unknown operator */
	    result = 0;
	    break;
    }

    return token_new(NUMBER, result, NULL, NULL);
}

token *expr(lexer *l);

token *factor(lexer *l)
{
    token *result;
    token *curr = lexer_peek(l);
    toktype curr_type = token_gettype(curr);
    if (curr_type == LPAREN) {
        expect(l, LPAREN);
        result = expr(l);
        expect(l, RPAREN);
    }
    else {
        expect(l, NUMBER);
        result = curr;
}
    return result;
}

token *term(lexer *l)
{
    token *result = factor(l);
    token *curr = lexer_peek(l);
    toktype curr_type = token_gettype(curr);
    while (curr_type == TIMES || curr_type == DIVIDE) {
        token *op, *b;
        if (curr_type == TIMES) {
            expect(l, TIMES);
            op = curr;
            b = term(l);
        }
        else {
            expect(l, DIVIDE);
            op = curr;
            b = term(l);
        }
        result = calc(l, op, result, b);
        token_delete(op); token_delete(b);
        curr = lexer_peek(l);
        curr_type = token_gettype(curr);
    }
    return result;
}

token *expr(lexer *l)
{
    token *result = term(l);
    if (token_gettype(result) == ERR) {
        return result;
    }
    token *curr = lexer_peek(l);
    toktype curr_type = token_gettype(curr);
    while (curr_type == PLUS || curr_type == MINUS) {
        token *op, *b;
        if (curr_type == PLUS) {
            expect(l, PLUS);
            op = curr;
            b = term(l);
        }
        else {
            expect(l, MINUS);
            op = curr;
            b = term(l);
        }
        result = calc(l, op, result, b);
        token_delete(op); token_delete(b);
        curr = lexer_peek(l);
        curr_type = token_gettype(curr);
    }
    return result;
}

