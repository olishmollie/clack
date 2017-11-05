#include <stdio.h>
#include <stdlib.h>

#include "../headers/token.h"
#include "../headers/ast.h"

ast *ast_binop(token *root, ast *left, ast *right)
{
    ast *a = malloc(sizeof(ast));
    if (a) {
	a->root = root;
	a->left = left;
	a->right = right;
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
    }
    return a;
}

ast *ast_num(token *n)
{
    ast *a = malloc(sizeof(ast));
    if (a) {
	a->root = n;
	a->left = a->right = NULL;
    }
    return a;
}

ast *ast_var(token *v)
{
    ast *a = malloc(sizeof(ast));
    if (a) {
	a->root = v;
	a->left = a->right = NULL;
    }
    return a;
}

ast *ast_err(token *e)
{
    ast *a = malloc(sizeof(ast));
    if (a) {
	a->root = e;
	a->left = a->right = NULL;
    }
    return a;
}

toktype ast_gettype(ast *a)
{
    return a ? token_gettype(a->root) : NOOP;
}

static void ast_print_recurse(ast *a, int n, char *ident)
{
    if (!a) return;
    char *rootstr = token_str(a->root);
    char* indent = malloc(n*sizeof(char));

    for (int i = 0; i < n; i++) {
	indent[i] = ' ';
    }

    if (ident) {
	printf("%s%s: %s\n", indent, ident, rootstr);
    } else {
	printf("%s%s\n", indent, rootstr);
    }

    ast_print_recurse(a->left, n+1, "Left");
    ast_print_recurse(a->right, n+1, "Right");

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
    free(a);
}
