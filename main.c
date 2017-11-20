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

    char *fname = argv[1];
    FILE *fptr = fopen(fname, "r");
    if (fptr == NULL) {
        fprintf(stderr, "error: unable to open file %s\n", fname);
        exit(1);
    }

    ssize_t read;
    size_t size = 0;
    char *line = NULL;
    char buf[MAXBUFSIZE];

    while ((read = getline(&line, &size, fptr)) != -1) {
        strcat(buf, line);
    }
    free(line);

    table *t = table_new();
    lexer *l = lexer_new(buf);
    statement_list *res = parse(l);

    int i;
    for (i = 0; i < res->num_statements; i++) {
        statement_print(res->statements[i]);
    }
    statement_list_delete(res);
    lexer_delete(l);
    table_delete(t);

    return 0;
}
