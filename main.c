#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <editline/readline.h>

#include "headers/ast.h"
#include "headers/parser.h"
#include "headers/table.h"
#include "headers/interpreter.h"

int main(void)
{
    table *t = table_new();

    while (1) {
	char *input = readline("fig>> ");
	add_history(input);

	lexer *l = lexer_new(input);

	while (!lexer_eof(l)) {
	    ast *res = parse(l);
	    printf("%d\n", visit(res, t));
	    ast_delete(res);
	}

	lexer_delete(l);
    }

    table_delete(t);

    return 0;
}
