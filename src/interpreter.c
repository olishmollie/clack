#include <stdio.h>
#include <stdlib.h>

#include "../headers/token.h"
#include "../headers/ast.h"
#include "../headers/parser.h"
#include "../headers/table.h"
#include "../headers/interpreter.h"

void visit_statement_list(statement_list *sl, table *t)
{
    int i;
    for (i = 0; i < sl->num_statements; i++) {
	visit(sl->statements[i], t);
    }
}

int visit(statement *a, table *t)
{
    toktype curr_type = statement_gettype(a);
    int val; char *name = NULL, *err = NULL;
    switch (curr_type) {
	case PLUS:
	    return visit(a->left, t) + visit(a->right, t);
	case MINUS:
	    return visit(a->left, t) - visit(a->right, t);
	case TIMES:
	    return visit(a->left, t) * visit(a->right, t);
	case DIVIDE:
	    return visit(a->left, t) / visit(a->right, t);
    case NUMBER:
	    return token_getvalue(a->root);
	case EQUALS:
	    val = visit(a->right, t);
	    name = token_getname(a->left->root);
	    table_insert(t, name, val);
	    return val;
	case IDENT:
	    name = token_getname(a->root);
	    val = table_getvalue(t, name);
	    return val;
	case ERR:
	    err = token_geterr(a->root);
	    printf("%s\n", err);
        return 0;
	default:
	    return 0;
    }
}
