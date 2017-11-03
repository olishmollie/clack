#include <stdio.h>
#include <stdlib.h>

#include "../headers/token.h"
#include "../headers/ast.h"
#include "../headers/parser.h"
#include "../headers/interpreter.h"

int visit(ast *a)
{
    toktype curr_type = ast_gettype(a);
    switch (curr_type) {
	case PLUS:
	    return visit(a->left) + visit(a->right);
	case MINUS:
	    return visit(a->left) - visit(a->right);
	case TIMES:
	    return visit(a->left) * visit(a->right);
	case DIVIDE:
	    return visit(a->left) / visit(a->right);
	case NUMBER:
	    return token_getvalue(a->root);
	default:
	    return 0;
    }
}
