#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "../headers/token.h"
#include "../headers/lexer.h"

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

static char curr_char(lexer *l)
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
    if (isspace(curr_char(l))) {
        advance(l);
    }
}

static int isop(char c)
{
    return c == '+' || c == '-' ||
	c == '*' || c == '/';
}

static int isparen(char c)
{
    return c == '(' || c == ')';
}

static toktype chartype(char c)
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

static token *read_digit(lexer *l)
{
    token *t;
    int i = 0, val = 0;
    char c = curr_char(l);
    while (chartype(c) == NUMBER) {
        c = advance(l);
        val *= (10 * (i + 1));
        val += c - '0';
        c = curr_char(l);
    }
    t = token_new(NUMBER, val, NULL, NULL);
    return t;
}

static token *read_ident(lexer *l)
{
    char* str = malloc(MAXBUFSIZE*sizeof(char));
    char c = curr_char(l);
    while (chartype(c) == IDENT) {
        c = advance(l);
        char tmp[2] = {c};
        strcat(str, tmp);
	c = curr_char(l);
    }
    str = realloc(str, strlen(str));
    return token_new(IDENT, 0, str, NULL);
}

static token *read_next(lexer *l)
{
    skip_whitespace(l);
    token *t = NULL;
    char c = curr_char(l);
    if (c == '\0') {
        t = token_new(END, 0, NULL, NULL);
    }
    else if (isdigit(c)) {
        return read_digit(l);
    }
    else if (isop(c)) {
        char a = advance(l);
        t = token_new(chartype(a), 0, NULL, NULL);
    }
    else if (isparen(c)) {
        char a = advance(l);
        t = token_new(chartype(a), 0, NULL, NULL);
    }
    else if (isalpha(c)) {
//        char a = advance(l);
//        t = token_new(chartype(a), a, NULL);
        return read_ident(l);
    }
    /* TODO: Error handling */
    return t;
}

token *lexer_peek(lexer* l)
{
    if (l->currtok == NULL)
        l->currtok = read_next(l);
    return l->currtok;
}

token *lexer_next(lexer* l)
{
    token *tmp = lexer_peek(l);
    l->currtok = read_next(l);
    return tmp;
}

int lexer_getline(lexer* l)
{
    return l->line;
}

int lexer_getcol(lexer* l)
{
    return l->col;
}

int lexer_eof(lexer* l)
{
    if (l->currtok == NULL)
        return 0;
    return token_gettype(l->currtok) == END;
}

void lexer_halt(lexer* l)
{
    l->pos = l->size; /* force eof */
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
