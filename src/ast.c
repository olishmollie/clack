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

ast *ast_branch(token *root, ast *cond, astlist *ifbody, astlist *elsebody)
{
    ast *b = malloc(sizeof(ast));
    if (b) {
        b->root = root;
        b->left = cond;
        b->right = NULL;
        b->ifbody = ifbody;
        b->elsebody = elsebody;
    }
    return b;
}

ast *ast_binop(token *root, ast *left, ast *right)
{
    ast *a = malloc(sizeof(ast));
    if (a) {
        a->root = root;
        a->left = left;
        a->right = right;
        a->ifbody = NULL;
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
        a->ifbody = NULL;
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
        a->ifbody = NULL;
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
        a->ifbody = NULL;
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
        a->ifbody = NULL;
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
        a->ifbody = NULL;
        a->elsebody = NULL;
    }
    return a;
}

void astlist_print(astlist*, int);
void ast_branch_print(ast*, int);

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
    } else {
        ast_print_recurse(a->left, n+1, "Left:");
        ast_print_recurse(a->right, n+1, "Right:");

    }

    free(rootstr); free(indent);
}

void ast_branch_print(ast* b, int n)
{
    ast_print_recurse(b->left, n, "Condition:");

    char* indent = malloc(2*n*sizeof(char)+1);

    int i;
    for (i = 0; i < 2*n; i++) {
        indent[i] = ' ';
    }
    indent[i] = '\0';

    printf("%sIFBody:\n", indent);
    astlist_print(b->ifbody, n+1);
    printf("%sELSEBody:\n", indent);
    astlist_print(b->elsebody, n+1);
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

