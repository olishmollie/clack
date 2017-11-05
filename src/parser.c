#include <stdio.h>
#include <stdlib.h>

#include "../headers/token.h"
#include "../headers/lexer.h"
#include "../headers/ast.h"

ast *type_error(lexer *l, toktype unexpected)
{
    char *msg = malloc(MAXBUFSIZE*sizeof(char));
    token *t = token_new(ERR, -1, NULL, msg);
    return ast_var(t);
}

// TODO: Throw exception
static void expect(lexer *l, toktype expected)
{
    token *curr = lexer_peek(l);
    toktype actual = token_gettype(curr);
    if (actual == expected) {
	lexer_next(l);
    } else {
	printf("ERROR: Parsing has failed with type %s\n", tokname(actual));
	lexer_halt(l);
	exit(1);
    }
}

ast *expr(lexer *l);

ast *variable(lexer *l)
{
    token *curr = lexer_peek(l);
    expect(l, IDENT);
    return ast_var(curr);
}

ast *factor(lexer *l)
{
    ast *result;
    token *curr = lexer_peek(l);
    toktype curr_type = token_gettype(curr);
    if (curr_type == LPAREN) {
        expect(l, LPAREN);
        result = expr(l);
        expect(l, RPAREN);
    } else if (curr_type == NUMBER) {
        expect(l, NUMBER);
        result = ast_num(curr);
    } else if (curr_type == PLUS) {
	expect(l, PLUS);
	result = ast_unaryop(curr, factor(l));
    } else if (curr_type == MINUS) {
        expect(l, MINUS);
        result = ast_unaryop(curr, factor(l));
    } else if (curr_type == IDENT) {
        result = variable(l);
    } else {
        /* TODO: Error handling */
        result = type_error(l, NIL);
    }
    return result;
}

ast *term(lexer *l)
{
    ast *left = factor(l);
    token *curr = lexer_peek(l);
    toktype curr_type = token_gettype(curr);
    while (curr_type == TIMES || curr_type == DIVIDE) {
        token *op;
        ast *right;
        if (curr_type == TIMES) {
            expect(l, TIMES);
            op = curr;
            right = factor(l);
        } else {
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
    while (curr_type == PLUS || curr_type == MINUS || curr_type == EQUALS) {
        token *op; ast *right;
        if (curr_type == PLUS) {
            expect(l, PLUS);
            op = curr;
            right = term(l);
        } else if (curr_type == MINUS) {
            expect(l, MINUS);
            op = curr;
            right = term(l);
        } else {
            expect(l, EQUALS);
            op = curr;
            right = expr(l);
        }
        left = ast_binop(op, left, right);
        curr = lexer_peek(l);
        curr_type = token_gettype(curr);
    }
    return left;
}

ast *parse(lexer *l) {
    return expr(l);
}

