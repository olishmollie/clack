#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/token.h"
#include "../headers/ast.h"
#include "../headers/parser.h"
#include "../headers/table.h"
#include "../headers/interpreter.h"

const char *builtins[] = { "print" };
int num_builtins = sizeof(builtins)/sizeof(char*);

void visit_prog(ast *sl, table *t)
{
    int i;
    for (i = 0; i < sl->num_children; i++) {
        visit_binop(sl->children[i], t);
    }
}

static int is_builtin(char *name)
{
    int i;
    for (i = 0; i < num_builtins; i++) {
        if (strcmp(builtins[i], name) == 0) {
            return 1;
        }
    }
    return 0;
}

void builtin_print(ast *f, table *t)
{
    int i;
    for (i = 0; i < f->num_params;  i++) {
        int delim = i < f->num_params - 1 ? ' ' : '\n';
        printf("%d%c", visit_binop(f->params[i], t), delim);
    }
}

void visit_builtin(ast *f, table *t)
{
    if (strcmp(token_getname(f->root), "print") == 0) {
        builtin_print(f, t);
    }
}

int visit_binop(ast *a, table *t)
{
    toktype curr_type = ast_gettype(a);
    int val; char *name = NULL, *err = NULL;
    switch (curr_type) {
    case PLUS:
        return visit_binop(a->left, t) + visit_binop(a->right, t);
    case MINUS:
        return visit_binop(a->left, t) - visit_binop(a->right, t);
    case TIMES:
        return visit_binop(a->left, t) * visit_binop(a->right, t);
    case DIVIDE:
        return visit_binop(a->left, t) / visit_binop(a->right, t);
        case NUMBER:
        return token_getvalue(a->root);
    case EQUALS:
        val = visit_binop(a->right, t);
        name = token_getname(a->left->root);
        table_insert(t, name, val);
        return val;
    case IDENT:
        name = token_getname(a->root);
        if (is_builtin(name)) {
            visit_builtin(a, t);
        } else {
            val = table_getvalue(t, name);
            return val;
        }
        break;
    case ERR:
        err = token_geterr(a->root);
        fprintf(stderr, "%s\n", err);
        return 0;
    default:
        return 0;
    }
    return 0;
}

