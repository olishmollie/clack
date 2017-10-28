#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <editline/readline.h>

#include "istream.h"

int main(void)
{
    while (1) {
	char *input = readline(">> ");
	add_history(input);

	istream *is = istream_new(input);

	while (!istream_eof(is)) {
	    printf("%c\n", istream_next(is));
	}

	istream_delete(is);
    }

    return 0;
}
