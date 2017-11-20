#include <stdio.h>
#include <stdlib.h>

#include "../headers/token.h"
#include "../headers/ast.h"

/* STATEMENTS */

statement *statement_binop(token *root, statement *left, statement *right)
{
    statement *a = malloc(sizeof(statement));
    if (a) {
	a->root = root;
	a->left = left;
	a->right = right;
    }
    return a;
}

statement *statement_unaryop(token *root, statement *next)
{
    statement *a = malloc(sizeof(statement));
    if (a) {
	a->root = root;
	a->left = NULL;
	a->right = next;
    }
    return a;
}

statement *statement_num(token *n)
{
    statement *a = malloc(sizeof(statement));
    if (a) {
	a->root = n;
	a->left = a->right = NULL;
    }
    return a;
}

statement *statement_var(token *v)
{
    statement *a = malloc(sizeof(statement));
    if (a) {
	a->root = v;
	a->left = a->right = NULL;
    }
    return a;
}

statement *statement_err(token *e)
{
    statement *a = malloc(sizeof(statement));
    if (a) {
	a->root = e;
	a->left = a->right = NULL;
    }
    return a;
}

toktype statement_gettype(statement *a)
{
    return a ? token_gettype(a->root) : NOOP;
}

static void statement_print_recurse(statement *a, int n, char *ident)
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

    statement_print_recurse(a->left, n+1, "Left");
    statement_print_recurse(a->right, n+1, "Right");

    free(rootstr); free(indent);
}

void statement_print(statement *a)
{
    statement_print_recurse(a, 0, NULL);
}

void statement_delete(statement *a)
{
    if (!a) return;
    token_delete(a->root);
    statement_delete(a->left);
    statement_delete(a->right);
    free(a);
}

/* STATEMENT LISTS */
statement_list *statement_list_new()
{
    statement_list *b = malloc(sizeof(statement_list));
    if (b) {
	b->statements = malloc(1000*sizeof(statement*));
	b->num_statements = 0;
    }
    return b;
}

void statement_list_add_statement(statement_list *sl, statement *s)
{
    if (sl->num_statements < 1000) {
	sl->statements[sl->num_statements++] = s;
    } else {
	fprintf(stderr, "error: too many statements\n");
    }
}

void statement_list_delete(statement_list *b)
{
    if (b) {
	if (b->statements) {
	    for (int i = 0; i < b->num_statements; i++) {
		statement_delete(b->statements[i]);
	    }
	    free(b->statements);
	}
	free(b);
    }
}
