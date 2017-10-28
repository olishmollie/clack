#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"
#include "token.h"

#define MAXBUFSIZE 32

struct lexer_t {
    char* input;
    int size, pos, line, col;
    token *currtok;
};

lexer *lexer_new(char* input)
{
    lexer *l = malloc(sizeof(lexer));
    if (l) {
	l->input = malloc(MAXBUFSIZE*sizeof(char));
	if (l->input) {
	    strncpy(l->input, input, (size_t)MAXBUFSIZE);
	    l->size = strlen(input);
	    l->pos = 0; l->col = 0; l->line = 1;
	    l->currtok = NULL;
	} else {
	    free(l);
	    return NULL;
	}
    }
    return l;
}

static char currchar(lexer* l)
{
    if (l->pos < l->size)
	return l->input[l->pos];
    else
	return '\0';
}

static char advance(lexer* l)
{
    if (l->pos < l->size) {
	char next = l->input[l->pos++];
	if (next == '\n')
	    l->col = 0; l->line++;
	return next;
    } else {
	return '\0';
    }
}

static void skip_whitespace(lexer *l)
{
    if (isspace(currchar(l))) {
	advance(l);
    }
}

static int isop(char c)
{
    return c == '+' || c == '-' ||
	c == '*' || c == '/';
}

static toktype optype(c)
{
    switch (c) {
	case '+': return PLUS;
	case '-': return MINUS;
	case '*': return TIMES;
	case '/': return DIVIDE;
	default: return -1;
    }
}

static token *readnext(lexer* l)
{
    skip_whitespace(l);
    token *t = NULL;
    char c = currchar(l);
    if (c == '\0')
	t = token_new(END, 0);
    if (isdigit(c)) {
	char a = advance(l);
	int val = a - '0';
	t = token_new(NUMBER, val);
    }
    if (isop(c)) {
	char a = advance(l);
	t = token_new(optype(a), 0);
    }
    return t;
}

token *lexer_peek(lexer* l)
{
    if (l->currtok == NULL)
	l->currtok = readnext(l);
    return l->currtok;
}

token *lexer_next(lexer* l)
{
    token *tmp = l->currtok;
    l->currtok = readnext(l);
    return tmp;
}

int lexer_eof(lexer* l)
{
    return token_gettype(lexer_peek(l)) == END;
}

void lexer_delete(lexer *l)
{
    if (l) {
	if (l->input)
	    free(l->input);
	free(l);
    }
}
