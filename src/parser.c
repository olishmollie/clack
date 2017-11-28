#include <stdio.h>
#include <string.h>

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

ast *symbol(lexer *l);
ast *sentence(lexer*);

ast *assign_stmt(lexer *l, token *var)
{
    ast *left = ast_var(var);
    token *eq = lexer_currtok(l);
    expect(l, ASSIGN);
    ast *right = sentence(l);

    return ast_binop(eq, left, right);
}

ast *symbol(lexer *l)
{
    token *curr = lexer_currtok(l);
    expect(l, IDENT);
    if (token_gettype(lexer_currtok(l)) == ASSIGN) {
        return assign_stmt(l, curr);
    } else {
        return ast_var(curr);
    }
}

ast *factor(lexer *l)
{
    ast *result = NULL;
    token *curr = lexer_currtok(l);
    toktype curr_type = token_gettype(curr);
    if (curr_type == LPAREN) {
        expect(l, LPAREN);
        result = sentence(l);
        expect(l, RPAREN);
    } else if (curr_type == INT) {
        expect(l, INT);
        result = ast_num(curr);
    } else if (curr_type == FLOAT) {
        expect(l, FLOAT);
        result = ast_num(curr);
    } else if (curr_type == BOOL) {
        expect(l, BOOL);
        result = ast_num(curr);
    } else if (curr_type == PLUS) {
        expect(l, PLUS);
        result = ast_unaryop(curr, factor(l));
    } else if (curr_type == MINUS) {
        expect(l, MINUS);
        result = ast_unaryop(curr, factor(l));
    } else if (curr_type == IDENT) {
        result = symbol(l);
    } else if (curr_type == STRING) {
        expect(l, STRING);
        result = ast_str(curr);
    } else if (curr_type == SEMI) {
        expect(l, SEMI);
        result = ast_noop();
    } else {
        error(l, curr);
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
    while (curr_type == PLUS || curr_type == MINUS) {
        token *op; ast *right;
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

ast *sentence(lexer *l)
{
    ast *left = expr(l);
    token *curr = lexer_currtok(l);
    toktype curr_type = token_gettype(curr);
    while (curr_type == EQUALS || curr_type == LT || curr_type == LTE
           || curr_type == GT || curr_type == GTE || curr_type == NEQUALS) {
        token *op = NULL; ast *right = NULL;
        if (curr_type == EQUALS) {
            expect(l, EQUALS);
            op = curr;
            right = sentence(l);
        } else if (curr_type == LT) {
            expect(l, LT);
            op = curr;
            right = sentence(l);
        } else if (curr_type == LTE) {
            expect(l, LTE);
            op = curr;
            right = sentence(l);
        } else if (curr_type == GT) {
            expect(l, GT);
            op = curr;
            right = sentence(l);
        } else if (curr_type == GTE) {
            expect(l, GTE);
            op = curr;
            right = sentence(l);
        } else {
            expect(l, NEQUALS);
            op = curr;
            right = sentence(l);
        }
        left = ast_binop(op, left, right);
        curr = lexer_currtok(l);
        curr_type = token_gettype(curr);
    }

    return left;
}

ast *branch(lexer*, token*);

ast *stmt(lexer *l) {
    token *curr = lexer_currtok(l);
    toktype curr_type = token_gettype(curr);
    if (curr_type == END)
        return NULL;
    if (curr_type == IF)
        return branch(l, curr);
    ast *s = sentence(l);
    expect(l, SEMI);
    return s;
}

astlist *stmt_list(lexer *l, toktype finish)
{
    astlist *sl = astlist_new();

    astlist_addchild(sl, stmt(l));
    if (err) {
        astlist_delete(sl);
        return NULL;
    }

    /* TODO: Refactor */
    while (token_gettype(lexer_currtok(l)) != finish) {
        astlist_addchild(sl, stmt(l));
        if (err) {
            astlist_delete(sl);
            return NULL;
        }
    }
    expect(l, finish);

    return sl;
}

ast *branch(lexer *l, token *b) {
    ast *cond;
    astlist *ifbody, *elsebody = NULL;

    expect(l, IF);
    cond = sentence(l);
    expect(l, LBRACE);
    ifbody = stmt_list(l, RBRACE);

    token *curr = lexer_currtok(l);
    toktype curr_type = token_gettype(curr);
    while (curr_type == ELSE) {
        expect(l, ELSE);

        elsebody = astlist_new();

        curr = lexer_currtok(l);
        curr_type = token_gettype(curr);
        if (curr_type == IF) {
            astlist_addchild(elsebody, branch(l, curr));
        } else {
            expect(l, LBRACE);
            astlist_append(elsebody, stmt_list(l, RBRACE));
        }
    }

    return ast_branch(b, cond, ifbody, elsebody);
}

astlist *parse(lexer *l) {
    err = 0;
    return stmt_list(l, END);
}

