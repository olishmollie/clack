#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <editline/readline.h>

#include "headers/ast.h"
#include "headers/parser.h"
#include "headers/table.h"
#include "headers/interpreter.h"

void repl()
{
    table *t = table_new();

    while (1) {
	char *input = readline("fig>> ");
	add_history(input);

	lexer *l = lexer_new(input);

	while (!lexer_eof(l)) {
	    statement_list *res = parse(l);
	    int i;
	    for (i = 0; i < res->num_statements; i++) {
		statement_print(res->statements[i]);
	    }
	    statement_list_delete(res);
	}

	lexer_delete(l);
    }

    table_delete(t);
}

int main(int argc, char **argv)
{
    if (argc <= 1) {
	repl();
	exit(0);
    }

    return 0;
}
