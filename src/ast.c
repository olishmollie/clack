#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/token.h"
#include "../headers/ast.h"

ast_stmtlist *ast_stmtlist_new()
{
    ast_stmtlist *sl = malloc(sizeof(ast_stmtlist));
    if (sl) {
        sl->num_children = 0;
    }
    return sl;
}

void ast_stmtlist_addchild(ast_stmtlist *sl, ast_stmt *s)
{
    if (sl->num_children < MAXSTATEMENTS) {
        sl->children[sl->num_children++] = s;
    } else {
        fprintf(stderr, "error: too many asts added to prog\n");
    }
}

ast_stmt *ast_binop(token *root, ast_stmt *left, ast_stmt *right)
{
    ast_stmt *a = malloc(sizeof(ast_stmt));
    if (a) {
        a->root = root;
        a->left = left;
        a->right = right;
    }
    return a;
}

ast_stmt *ast_unaryop(token *root, ast_stmt *next)
{
    ast_stmt *a = malloc(sizeof(ast_stmt));
    if (a) {
        a->root = root;
        a->left = NULL;
        a->right = next;
    }
    return a;
}

ast_stmt *ast_num(token *n)
{
    ast_stmt *a = malloc(sizeof(ast_stmt));
    if (a) {
        a->root = n;
        a->left = a->right = NULL;
    }
    return a;
}

ast_stmt *ast_var(token *v)
{
    ast_stmt *a = malloc(sizeof(ast_stmt));
    if (a) {
        a->root = v;
        a->left = a->right = NULL;
    }
    return a;
}

ast_stmt *ast_noop()
{
    ast_stmt *a = malloc(sizeof(ast_stmt));
    if (a) {
        a->root = token_new(NOOP, NULL, NULL);
        a->left = a->right = NULL;
    }
    return a;
}

static void ast_stmt_print_recurse(ast_stmt *a, int n, char *ident)
{
    if (!a) return;
    char *rootstr = token_str(a->root);
    char* indent = malloc(2*n*sizeof(char)+1);

    int i;
    for (i = 0; i < 2*n; i++) {
        indent[i] = ' ';
    }
    indent[i] = '\0';

    if (ident) {
        printf("%s%s %s\n", indent, ident, rootstr);
    } else {
        printf("%s%s\n", indent, rootstr);
    }

    ast_stmt_print_recurse(a->left, n+1, "Left:");
    ast_stmt_print_recurse(a->right, n+1, "Right:");

    free(rootstr); free(indent);
}

void ast_stmtlist_print(ast_stmtlist *sl)
{
    int i;
    for (i = 0; i < sl->num_children; i++)
        ast_stmt_print(sl->children[i]);
}

void ast_stmt_print(ast_stmt *a)
{
    ast_stmt_print_recurse(a, 0, NULL);
}

void ast_stmtlist_delete(ast_stmtlist *sl)
{
    if (sl) {
        int i;
        for (i = 0; i < sl->num_children; i++)
            ast_stmt_delete(sl->children[i]);
        free(sl);
    }
}

void ast_stmt_delete(ast_stmt *a)
{
    if (!a) return;
    token_delete(a->root);
    ast_stmt_delete(a->left);
    ast_stmt_delete(a->right);

    free(a);
}

