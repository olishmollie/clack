#include <stdio.h>
#include <stdlib.h>

#include "../headers/token.h"
#include "../headers/ast.h"

struct ast_t {
    token *root;
    ast *left;
    ast *right;
};

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

ast *ast_num(token *n)
{
    ast *a = malloc(sizeof(ast));
    if (a) {
	a->root = n;
	a->left = a->right = NULL;
    }
    return a;
}

void ast_print(ast *a, int n, char *ident)
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

    ast_print(a->left, n+1, "Left");
    ast_print(a->right, n+1, "Right");

    free(rootstr); free(indent);
}

void ast_delete(ast *a)
{
    if (!a) return;
    token_delete(a->root);
    ast_delete(a->left);
    ast_delete(a->right);
    free(a);
}
