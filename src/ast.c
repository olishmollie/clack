#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/token.h"
#include "../headers/ast.h"

astlist *astlist_new()
{
    astlist *sl = malloc(sizeof(astlist));
    if (sl) {
        sl->num_children = 0;
    }
    return sl;
}

void astlist_addchild(astlist *sl, ast *s)
{
    if (sl->num_children < MAXSTATEMENTS) {
        sl->children[sl->num_children++] = s;
    } else {
        fprintf(stderr, "error: too many asts added to prog\n");
    }
}

void astlist_append(astlist *dest, astlist *src)
{
    int i;
    for (i = 0; i < src->num_children; i++) {
        astlist_addchild(dest, src->children[i]);
    }
}

ast *ast_branch(token *root, ast *cond, astlist *body, astlist *elsebody)
{
    ast *b = malloc(sizeof(ast));
    if (b) {
        b->root = root;
        b->left = cond;
        b->right = NULL;
        b->body = body;
        b->elsebody = elsebody;
    }
    return b;
}

ast *ast_loop(token *root, ast *cond, astlist *body)
{
    ast *l = malloc(sizeof(ast));
    if (l) {
        l->root = root;
        l->left = cond;
        l->right = NULL;
        l->body = body;
        l->elsebody = NULL;
    }
    return l;
}

ast *ast_binop(token *root, ast *left, ast *right)
{
    ast *a = malloc(sizeof(ast));
    if (a) {
        a->root = root;
        a->left = left;
        a->right = right;
        a->body = NULL;
        a->elsebody = NULL;
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
        a->body = NULL;
        a->elsebody = NULL;
    }
    return a;
}

ast *ast_num(token *n)
{
    ast *a = malloc(sizeof(ast));
    if (a) {
        a->root = n;
        a->left = a->right = NULL;
        a->body = NULL;
        a->elsebody = NULL;
    }
    return a;
}

ast *ast_var(token *v)
{
    ast *a = malloc(sizeof(ast));
    if (a) {
        a->root = v;
        a->left = a->right = NULL;
        a->body = NULL;
        a->elsebody = NULL;
    }
    return a;
}

ast *ast_str(token *s)
{
    ast *a = malloc(sizeof(ast));
    if (a) {
        a->root = s;
        a->left = a->right = NULL;
        a->body = NULL;
        a->elsebody = NULL;
    }
    return a;
}

ast *ast_noop()
{
    ast *a = malloc(sizeof(ast));
    if (a) {
        a->root = token_new(NOOP, NULL, NULL);
        a->left = a->right = NULL;
        a->body = NULL;
        a->elsebody = NULL;
    }
    return a;
}

void astlist_print(astlist*, int);
void ast_branch_print(ast*, int);
void ast_loop_print(ast*, int);

static void ast_print_recurse(ast *a, int n, char *ident)
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

    if (token_gettype(a->root) == IF) {
        ast_branch_print(a, n+1);
    } else if (token_gettype(a->root) == WHILE) {
        ast_loop_print(a, n+1);
    } else {
        ast_print_recurse(a->left, n+1, "LEFT:");
        ast_print_recurse(a->right, n+1, "RIGHT:");

    }

    free(rootstr); free(indent);
}

void ast_branch_print(ast* b, int n)
{
    ast_print_recurse(b->left, n, "IF:");

    char* indent = malloc(2*n*sizeof(char)+1);

    int i;
    for (i = 0; i < 2*n; i++) {
        indent[i] = ' ';
    }
    indent[i] = '\0';

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

    char* indent = malloc(2*n*sizeof(char)+1);

    int i;
    for (i = 0; i < 2*n; i++) {
        indent[i] = ' ';
    }
    indent[i] = '\0';

    printf("%sDO:\n", indent);
    astlist_print(l->body, n+1);

    free(indent);
}

void astlist_print(astlist *sl, int n)
{
    int i;
    for (i = 0; i < sl->num_children; i++)
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
        for (i = 0; i < sl->num_children; i++)
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

