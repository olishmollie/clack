#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/token.h"
#include "../headers/ast.h"

astlist *astlist_new()
{
    astlist *sl = malloc(sizeof(astlist));
    if (sl) {
        sl->nchildren = 0;
    }
    return sl;
}

void astlist_addchild(astlist *sl, ast *s)
{
    if (sl->nchildren < MAXSTATEMENTS) {
        sl->children[sl->nchildren++] = s;
    } else {
        fprintf(stderr, "error: too many asts added to prog\n");
    }
}

void astlist_append(astlist *dest, astlist *src)
{
    int i;
    for (i = 0; i < src->nchildren; i++) {
        astlist_addchild(dest, src->children[i]);
    }
}

ast *ast_new() {
    ast *a = malloc(sizeof(ast));
    if (a) {
        a->root = NULL;
        a->right = a->left = NULL;
        a->args = a->body = a->elsebody = NULL;
    }
    return a;
}

ast *ast_branch(token *root, ast *cond, astlist *body, astlist *elsebody)
{
    ast *b = ast_new();
    if (b) {
        b->root = root;
        b->left = cond;
        b->body = body;
        b->elsebody = elsebody;
    }
    return b;
}

ast *ast_loop(token *root, ast *cond, astlist *body)
{
    ast *l = ast_new();
    if (l) {
        l->root = root;
        l->left = cond;
        l->body = body;
    }
    return l;
}

ast *ast_funcall(token *root, astlist *args)
{
    ast *fc = ast_new();
    if (fc) {
        fc->root = root;
        fc->args = args;
    }
    return fc;
}

ast *ast_binop(token *root, ast *left, ast *right)
{
    ast *a = ast_new();
    if (a) {
        a->root = root;
        a->left = left;
        a->right = right;
    }
    return a;
}

ast *ast_unaryop(token *root, ast *next)
{
    ast *a = ast_new();
    if (a) {
        a->root = root;
        a->right = next;
    }
    return a;
}

ast *ast_num(token *n)
{
    ast *a = ast_new();
    if (a) {
        a->root = n;
    }
    return a;
}

ast *ast_var(token *v)
{
    ast *a = ast_new();
    if (a) {
        a->root = v;
    }
    return a;
}

ast *ast_str(token *s)
{
    ast *a = ast_new();
    if (a) {
        a->root = s;
    }
    return a;
}

ast *ast_fundef(token *name, astlist *args, astlist *body)
{
    ast *fn = ast_new();
    if (fn) {
        fn->root = name;
        fn->args = args;
        fn->body = body;
    }
    return fn;
}

ast *ast_noop()
{
    ast *a = ast_new();
    if (a) {
        a->root = token_new(NOOP, NULL, NULL);
    }
    return a;
}

static char *indent_str(int n)
{
    char *indent = malloc(2*n*sizeof(char));
    int i;
    for (i = 0; i < 2*n; i++) {
        indent[i] = ' ';
    }
    indent[i] = '\0';

    return indent;
}

static void ast_print_recurse(ast *a, int n, char *ident)
{
    if (!a) return;
    char *rootstr = token_str(a->root);
    char* indent = indent_str(n);

    if (ident) {
        printf("%s%s %s\n", indent, ident, rootstr);
    } else {
        printf("%s%s\n", indent, rootstr);
    }

    if (token_gettype(a->root) == IF) {
        ast_branch_print(a, n+1);
    } else if (token_gettype(a->root) == WHILE) {
        ast_loop_print(a, n+1);
    } else if (token_gettype(a->root) == FUNDEF) {
        ast_fundef_print(a, n+1);
    } else if (token_gettype(a->root) == FUNCALL) {
        ast_funcall_print(a, n+1);
    } else {
        ast_print_recurse(a->left, n+1, "LEFT:");
        ast_print_recurse(a->right, n+1, "RIGHT:");
    }

    free(rootstr); free(indent);
}

void ast_branch_print(ast* b, int n)
{
    ast_print_recurse(b->left, n, "IF:");

    char* indent = indent_str(n);

    printf("%sTHEN:\n", indent);
    astlist_print(b->body, n+1);
    if (b->elsebody) {
        printf("%sELSE:\n", indent);
        astlist_print(b->elsebody, n+1);
    }

    free(indent);
}

void ast_loop_print(ast *l, int n)
{
    ast_print_recurse(l->left, n, "WHILE:");

    char* indent = indent_str(n);

    printf("%sDO:\n", indent);
    astlist_print(l->body, n+1);

    free(indent);
}

void ast_funcall_print(ast *fc, int n)
{
    char* indent = indent_str(n);

    printf("%sPARAMS:\n", indent);
    astlist_print(fc->args, n+1);

    free(indent);
}

void ast_fundef_print(ast *fd, int n)
{
    char *indent = indent_str(n);

    printf("%sPARAMS:\n", indent);
    astlist_print(fd->args, n+1);

    printf("%sBODY:\n", indent);
    astlist_print(fd->body, n+1);
}

void astlist_print(astlist *sl, int n)
{
    int i;
    for (i = 0; i < sl->nchildren; i++)
        ast_print_recurse(sl->children[i], n, NULL);
}

void ast_print(ast *a)
{
    ast_print_recurse(a, 0, NULL);
}

void astlist_delete(astlist *sl)
{
    if (sl) {
        int i;
        for (i = 0; i < sl->nchildren; i++)
            ast_delete(sl->children[i]);
        free(sl);
    }
}

void ast_delete(ast *a)
{
    if (!a) return;
    token_delete(a->root);
    ast_delete(a->left);
    ast_delete(a->right);

    free(a);
}

