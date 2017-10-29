#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "../headers/lexer.h"
#include "../headers/token.h"

#define MAXBUFSIZE 150

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
        l->col++;
        if (next == '\n') {
            l->col = 0; l->line++;
        }
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

static int isparen(c)
{
    return c == '(' || c == ')';
}

static toktype ctoktype(c)
{
    if (isop(c)) {
	switch(c) {
	    case '+': return PLUS;
	    case '-': return MINUS;
	    case '*': return TIMES;
	    case '/': return DIVIDE;
	}
    }
    else if (isdigit(c)) {
        return NUMBER;
    }
    else if (isparen(c)) {
        switch(c) {
            case '(': return LPAREN;
            case ')': return RPAREN;
        }
    }
    else if (isalpha(c)) {
        return IDENT;
    }
    return -1;
}

static token *lexer_throw(lexer *l, toktype expected, toktype actual)
{
    l->pos = l->size; /* force eof */
    char *msg = malloc(MAXBUFSIZE*sizeof(char));
    snprintf(msg, MAXBUFSIZE, "Unexpected type %s, expected %s (%d:%d)",
             tokname(actual), tokname(expected), l->line, l->col);
    return token_new(ERR, 0, msg);
}

static token *readdigit(lexer *l)
{
    token *t;
    int i = 0, val = 0;
    char c = currchar(l);
    while (ctoktype(c) == NUMBER) {
        c = advance(l);
        val *= (10 * (i + 1));
        val += c - '0';
        c = currchar(l);
    }
    t = token_new(NUMBER, val, NULL);
    return t;
}

static token *readnext(lexer* l)
{
    skip_whitespace(l);
    token *t = NULL;
    char c = currchar(l);
    if (c == '\0') {
        t = token_new(END, 0, NULL);
    }
    else if (isdigit(c)) {
        return readdigit(l);
    }
    else if (isop(c)) {
        char a = advance(l);
        t = token_new(ctoktype(a), 0, NULL);
    }
    else if (isparen(c)) {
        char a = advance(l);
        t = token_new(ctoktype(a), 0, NULL);
    }
    else if (isalpha(c)) {
        char a = advance(l);
        t = token_new(ctoktype(a), a, NULL);
    }
    else {
        // TODO: Find a way to throw a better error
        lexer_throw(l, END, NUMBER);
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
    // TODO: Possible memory leak?
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
        if (l->currtok)
            token_delete(l->currtok);
	free(l);
    }
}
