#include <stdio.h>

#include "../headers/token.h"
#include "../headers/lexer.h"
#include "../headers/ast.h"

int err;

static void error(lexer *l, token *unexpected)
{
    err = 1;
    lexer_halt(l);
    if (token_gettype(unexpected) == ERR) {
        fprintf(stderr, "%s\n", token_geterr(unexpected));
    } else {
        char *toktype = tokname(token_gettype(unexpected));
        fprintf(stderr, "error: unexpected %s (%d|%d)\n",
                toktype, lexer_getline(l), lexer_getcol(l) - 1);
    }
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

ast_stmt *symbol(lexer *l);
ast_stmt *stmt(lexer*);

ast_stmt *assign_stmt(lexer *l, token *var)
{
    ast_stmt *left = ast_var(var);
    token *eq = lexer_currtok(l);
    expect(l, ASSIGN);
    ast_stmt *right = stmt(l);

    return ast_binop(eq, left, right);
}

ast_stmt *symbol(lexer *l)
{
    token *curr = lexer_currtok(l);
    expect(l, IDENT);
    if (token_gettype(lexer_currtok(l)) == ASSIGN) {
        return assign_stmt(l, curr);
    } else {
        return ast_var(curr);
    }
}

ast_stmt *factor(lexer *l)
{
    ast_stmt *result = NULL;
    token *curr = lexer_currtok(l);
    toktype curr_type = token_gettype(curr);
    if (curr_type == LPAREN) {
        expect(l, LPAREN);
        result = stmt(l);
        expect(l, RPAREN);
    } else if (curr_type == INT) {
        expect(l, INT);
        result = ast_num(curr);
    } else if (curr_type == FLOAT) {
        expect(l, FLOAT);
        result = ast_num(curr);
    } else if (curr_type == PLUS) {
        expect(l, PLUS);
        result = ast_unaryop(curr, factor(l));
    } else if (curr_type == MINUS) {
        expect(l, MINUS);
        result = ast_unaryop(curr, factor(l));
    } else if (curr_type == IDENT) {
        result = symbol(l);
    } else if (curr_type == NWLN) {
        expect(l, NWLN);
        result = ast_noop();
    } else {
        error(l, curr);
    }
    return result;
}

ast_stmt *term(lexer *l)
{
    ast_stmt *left = factor(l);
    token *curr = lexer_currtok(l);
    toktype curr_type = token_gettype(curr);
    while (curr_type == TIMES || curr_type == DIVIDE) {
        token *op;
        ast_stmt *right;
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

ast_stmt *expr(lexer *l)
{
    ast_stmt *left = term(l);
    token *curr = lexer_currtok(l);
    toktype curr_type = token_gettype(curr);
    while (curr_type == PLUS || curr_type == MINUS) {
        token *op; ast_stmt *right;
        if (curr_type == PLUS) {
            expect(l, PLUS);
            op = curr;
            right = term(l);
        } else {
            expect(l, MINUS);
            op = curr;
            right = term(l);
        }
        left = ast_binop(op, left, right);
        curr = lexer_currtok(l);
        curr_type = token_gettype(curr);
    }
    return left;
}

ast_stmt *stmt(lexer *l) {
    ast_stmt *left = expr(l);
    token *curr = lexer_currtok(l);
    toktype curr_type = token_gettype(curr);

    while (curr_type == EQUALS || curr_type == LT || curr_type == LTE
           || curr_type == GT || curr_type == GTE) {
        token *op = NULL; ast_stmt *right = NULL;
        if (curr_type == EQUALS) {
            expect(l, EQUALS);
            op = curr;
            right = expr(l);
        } else if (curr_type == LT) {
            expect(l, LT);
            op = curr;
            right = expr(l);
        } else if (curr_type == LTE) {
            expect(l, LTE);
            op = curr;
            right = expr(l);
        } else if (curr_type == GT) {
            expect(l, GT);
            op = curr;
            right = expr(l);
        } else if (curr_type == GTE) {
            expect(l, GTE);
            op = curr;
            right = expr(l);
        }
        left = ast_binop(op, left, right);
        curr = lexer_currtok(l);
        curr_type = token_gettype(curr);
    }

    return left;
}

ast_stmtlist *stmt_list(lexer *l)
{
    ast_stmtlist *sl = ast_stmtlist_new();

    ast_stmtlist_addchild(sl, stmt(l));
    if (err) {
        ast_stmtlist_delete(sl);
        return NULL;
    }

    while (token_gettype(lexer_currtok(l)) != END) {
        expect(l, NWLN);
        if (token_gettype(lexer_currtok(l)) == END)
            break;
        ast_stmtlist_addchild(sl, stmt(l));
        if (err) {
            ast_stmtlist_delete(sl);
            return NULL;
        }
    }

    return sl;
}

ast_stmtlist *parse(lexer *l) {
    err = 0;
    return stmt_list(l);
}

