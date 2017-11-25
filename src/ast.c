#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/token.h"
#include "../headers/ast.h"

ast *ast_prog()
{
    ast *b = malloc(sizeof(ast));
    if (b) {
        b->root = token_new(MAIN, NULL, NULL);
        b->left = b->right = NULL;
        b->children = malloc(MAXBUFSIZE*sizeof(ast*));
        b->num_children = 0;
        b->params = NULL;
        b->num_params = 0;
    }
    return b;
}

void ast_addparam(ast *f, ast *p)
{
    if (f->num_children < MAXPARAMS) {
        f->params[f->num_params++] = p;
    } else {
        fprintf(stderr, "error: too many params added to fn\n");
    }
}

void ast_addchild(ast *sl, ast *s)
{
    if (sl->num_children < MAXBUFSIZE) {
        sl->children[sl->num_children++] = s;
    } else {
        fprintf(stderr, "error: too many asts added to prog\n");
    }
}

ast *ast_funcall(token *name)
{
    ast *a = malloc(sizeof(ast));
    if (a) {
        a->root = name;
        a->left = a->right = NULL;
        a->params = calloc((size_t)MAXPARAMS, sizeof(ast*));
        a->num_params = 0;
        a->children = calloc((size_t)MAXBUFSIZE, sizeof(ast*));
        a->num_children = 0;
    }
    return a;
}

ast *ast_binop(token *root, ast *left, ast *right)
{
    ast *a = malloc(sizeof(ast));
    if (a) {
        a->root = root;
        a->left = left;
        a->right = right;
        a->children = NULL;
        a->num_children = 0;
        a->children = NULL;
        a->num_children = 0;
    }
    return a;
}

ast *ast_unaryop(token *root, ast *next)
{
    ast *a = malloc(sizeof(ast));
    if (a) {
        a->root = root;
        a->left = NULL;
        a->right = next;
        a->children = NULL;
        a->num_children = 0;
        a->children = NULL;
        a->num_children = 0;
    }
    return a;
}

ast *ast_num(token *n)
{
    ast *a = malloc(sizeof(ast));
    if (a) {
        a->root = n;
        a->left = a->right = NULL;
        a->children = NULL;
        a->num_children = 0;
        a->children = NULL;
        a->num_children = 0;
    }
    return a;
}

ast *ast_var(token *v)
{
    ast *a = malloc(sizeof(ast));
    if (a) {
        a->root = v;
        a->left = a->right = NULL;
        a->children = NULL;
        a->num_children = 0;
        a->children = NULL;
        a->num_children = 0;
    }
    return a;
}

ast *ast_err(token *e)
{
    ast *a = malloc(sizeof(ast));
    if (a) {
        a->root = e;
        a->left = a->right = NULL;
        a->children = NULL;
        a->num_children = 0;
        a->params = NULL;
        a->num_params = 0;
    }
    return a;
}

ast *ast_noop()
{
    ast *a = malloc(sizeof(ast));
    if (a) {
        a->root = token_new(NOOP, NULL, NULL);
        a->left = a->right = NULL;
        a->children = NULL;
        a->num_children = 0;
        a->params = NULL;
        a->num_params = 0;
    }
    return a;
}

toktype ast_gettype(ast *a)
{
    return a ? token_gettype(a->root) : NOOP;
}

static void ast_print_recurse(ast *a, int n, char *ident)
{
    /* TODO: Fix formatting/indentation */
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

    for (i = 0; i < a->num_children; i++) {
        ast_print_recurse(a->children[i], n+1, NULL);
    }

    ast_print_recurse(a->left, n+1, "Left:");
    ast_print_recurse(a->right, n+1, "Right:");

    free(rootstr); free(indent);
}

void ast_print(ast *a)
{
    ast_print_recurse(a, 0, NULL);
}

void ast_delete(ast *a)
{
    if (!a) return;
    token_delete(a->root);
    ast_delete(a->left);
    ast_delete(a->right);

    int i;
    for (i = 0; i < a->num_children; i++) {
        ast_delete(a->children[i]);
    }
    free(a);
}

