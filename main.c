#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <editline/readline.h>

#include "token.h"
#include "lexer.h"

int main(void)
{
    while (1) {
	char *input = readline(">> ");
	add_history(input);

	lexer *l = lexer_new(input);

	while (!lexer_eof(l)) {
	    token *t = lexer_next(l);
	    printf("%s\n", token_tostr(t));
	    token_delete(t);
	}

	lexer_delete(l);
    }

    return 0;
}
