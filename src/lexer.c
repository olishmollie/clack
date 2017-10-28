#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "../headers/lexer.h"
#include "../headers/token.h"

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

static int isparen(c)
{
    return c == '(' || c == ')';
}

static toktype parentype(c)
{
    switch(c) {
	case '(': return LPAREN;
	case ')': return RPAREN;
	default: return -1;
    }
}

static char expect(lexer* l, int(*predicate)(int))
{
    char c = currchar(l);
    if (predicate(c))
	return advance(l);
    else if (isspace(c) || c == '\0')
	return 0;
    else
	// TODO: Error handling
	return -1;
}

static token *lexer_throw(lexer *l)
{
    l->pos = l->size; /* force eof */
    return token_new(ERR, 0);
}

static token *readdigit(lexer *l)
{
    token *t;
    int i = 0, val = 0;
    char c = expect(l, &isdigit);
    while (isdigit(c)) {
	char s[2] = {c, '\0'};
	val *= (10 * (i + 1));
	val += c - '0';
	c = expect(l, &isdigit);
	if (c == -1)
	    return lexer_throw(l);
    }
    t = token_new(NUMBER, val);
    return t;
}

static token *readnext(lexer* l)
{
    skip_whitespace(l);
    token *t = NULL;
    char c = currchar(l);
    if (c == '\0') {
	t = token_new(END, 0);
    }
    else if (isdigit(c)) {
	return readdigit(l);
    }
    else if (isop(c)) {
	char a = advance(l);
	t = token_new(optype(a), 0);
    }
    else if (isparen(c)) {
	char a = advance(l);
	t = token_new(parentype(a), 0);
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
    if (!tmp)
	tmp = readnext(l);
    l->currtok = NULL;
    return tmp;
}

int lexer_eof(lexer* l)
{
    return l->pos >= l->size;
}

void lexer_delete(lexer *l)
{
    if (l) {
	if (l->input)
	    free(l->input);
	free(l);
    }
}
