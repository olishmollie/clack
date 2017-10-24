#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <editline/readline.h>

#define BUFSIZE 150

int main(void)
{
    while (1) {
	char *input = readline(">> ");
	add_history(input);
	printf("%s\n", input);
	free(input);
    }

    return 0;
}
