#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <editline/readline.h>

#include "headers/ast.h"
#include "headers/parser.h"

int main(void)
{
    while (1) {
        char *input = readline(">> ");
        add_history(input);

        lexer *l = lexer_new(input);

        while (!lexer_eof(l)) {
            ast *res = expr(l);
	    ast_print(res, 0, NULL);
	    ast_delete(res);
        }

        lexer_delete(l);
    }

    return 0;
}
