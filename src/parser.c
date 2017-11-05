#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/token.h"
#include "../headers/lexer.h"
#include "../headers/ast.h"

static ast *error(lexer *l, token *unexpected)
{
    lexer_halt(l);
    char *toktype = tokname(token_gettype(unexpected));
    char *msg = malloc(MAXBUFSIZE*sizeof(char));
    snprintf(msg, MAXBUFSIZE, "Parser error: unexpected %s (%d|%d)",
        toktype, lexer_getline(l), lexer_getcol(l) - 1); /* TODO: HACK!! */
    msg = realloc(msg, strlen(msg) + 1);
    token *err = token_new(ERR, 0, NULL, msg);
    return ast_err(err);
}

static void expect(lexer *l, toktype expected)
{
    token *curr = lexer_currtok(l);
    toktype actual = token_gettype(curr);
    if (actual == expected) {
        lexer_advance(l);
    } else {
        error(l, curr);
    }
}

ast *expr(lexer *l);

ast *variable(lexer *l)
{
    token *curr = lexer_currtok(l);
    expect(l, IDENT);
    return ast_var(curr);
}

ast *factor(lexer *l)
{
    ast *result = NULL;
    token *curr = lexer_currtok(l);
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
    } else if (curr_type == ERR) {
        expect(l, ERR);
        result = ast_err(curr);
    } else {
        result = error(l, curr);
    }
    return result;
}

ast *term(lexer *l)
{
    ast *left = factor(l);
    token *curr = lexer_currtok(l);
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
        curr = lexer_currtok(l);
        curr_type = token_gettype(curr);
    }
    return left;
}

ast *expr(lexer *l)
{
    ast *left = term(l);
    token *curr = lexer_currtok(l);
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
        curr = lexer_currtok(l);
        curr_type = token_gettype(curr);
    }
    return left;
}

ast *parse(lexer *l) {
    return expr(l);
}

