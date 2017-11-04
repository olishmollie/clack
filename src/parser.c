#include <stdlib.h>

#include "../headers/token.h"
#include "../headers/lexer.h"
#include "../headers/ast.h"

void type_error(lexer *l, toktype unexpected)
{
    char *msg = malloc(MAXBUFSIZE*sizeof(char));
    token_new(ERR, -1, NULL, msg);
}

// TODO: Throw exception
static void expect(lexer *l, toktype expected)
{
    token *curr = lexer_peek(l);
    toktype actual = token_gettype(curr);
    (actual == expected) ? lexer_next(l) : type_error(l, actual);
}

ast *expr(lexer *l);

ast *factor(lexer *l)
{
    ast *result;
    token *curr = lexer_peek(l);
    toktype curr_type = token_gettype(curr);
    if (curr_type == LPAREN) {
        expect(l, LPAREN);
        result = expr(l);
        expect(l, RPAREN);
    }
    else if (curr_type == NUMBER) {
        expect(l, NUMBER);
        result = ast_num(curr);
    } else if (curr_type == PLUS) {
	expect(l, PLUS);
	result = ast_unaryop(curr, factor(l));
    } else if (curr_type == MINUS) {
        expect(l, MINUS);
        ast *next = factor(l);
        result = ast_unaryop(curr, next);
    } else {
	/* TODO: Error handling */
	result = NULL;
    }
    return result;
}

ast *term(lexer *l)
{
    ast *left = factor(l);
    token *curr = lexer_peek(l);
    toktype curr_type = token_gettype(curr);
    while (curr_type == TIMES || curr_type == DIVIDE) {
        token *op; ast *right;
        if (curr_type == TIMES) {
            expect(l, TIMES);
            op = curr;
            right = factor(l);
        }
        else {
            expect(l, DIVIDE);
            op = curr;
            right = factor(l);
        }
	left = ast_binop(op, left, right);
        curr = lexer_peek(l);
        curr_type = token_gettype(curr);
    }
    return left;
}

ast *expr(lexer *l)
{
    ast *left = term(l);
    token *curr = lexer_peek(l);
    toktype curr_type = token_gettype(curr);
    while (curr_type == PLUS || curr_type == MINUS) {
        token *op; ast *right;
        if (curr_type == PLUS) {
            expect(l, PLUS);
            op = curr;
            right = term(l);
        }
        else {
            expect(l, MINUS);
            op = curr;
            right = term(l);
        }
        left = ast_binop(op, left, right);
        curr = lexer_peek(l);
        curr_type = token_gettype(curr);
    }
    return left;
}

