#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <editline/readline.h>

#include "headers/interpreter.h"

int main(void)
{
    while (1) {
        char *input = readline(">> ");
        add_history(input);

        lexer *l = lexer_new(input);

        while (!lexer_eof(l)) {
            token* res = expr(l);
            printf("%s\n", token_str(res));
	    token_delete(res);
        }

        lexer_delete(l);
    }

    return 0;
}
