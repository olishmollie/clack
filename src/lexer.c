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

static int is_space(char c)
{
    return c == ' ' || c == '\t';
}

static void skip_whitespace(lexer *l)
{
    while (is_space(curr_char(l))) {
        advance(l);
    }
}

static int is_newline(char c)
{
    return c == '\n' || c == '\r';
}

static int isop(char c)
{
    return c == '+' || c == '-' ||
	c == '*' || c == '/' || c == '=';
}

static int isparen(char c)
{
    return c == '(' || c == ')';
}

static int isbrace(char c)
{
    return c == '{' || c == '}';
}

static int iscomma(char c)
{
    return c == ',';
}

static int iscomment(char c)
{
    return c == '#';
}



static toktype chartype(char c)
{
    if (isop(c)) {
        switch(c) {
        case '+': return PLUS;
        case '-': return MINUS;
        case '*': return TIMES;
        case '/': return DIVIDE;
        case '=': return EQUALS;
	}
    } else if (isdigit(c)) {
        return NUMBER;
    } else if (isparen(c)) {
        switch(c) {
        case '(': return LPAREN;
        case ')': return RPAREN;
        }
    } else if (isbrace(c)) {
        switch(c) {
        case '{': return LBRACE;
        case '}': return RBRACE;
        }
    } else if (iscomma(c)) {
        return COMMA;
    } else if (isalpha(c)) {
        return IDENT;
    } else if (is_newline(c)) {
        return NWLN;
    } else if (iscomment(c)) {
        return COMMENT;
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

    token *i = token_new(IDENT, 0, str, NULL);
    return i;
}

static void skip_comment(lexer *l)
{
    char c = curr_char(l);

    while (chartype(c) != NWLN) {
        advance(l);
        c = curr_char(l);
    }

    advance(l);
}

static token *lexer_error(lexer *l, char c)
{
    char *msg = malloc(MAXBUFSIZE*sizeof(char));
    snprintf(msg, (size_t)MAXBUFSIZE,
	"Unexpected '%c' at (%d|%d)",
	c, lexer_getline(l), lexer_getcol(l));
    token *err = token_new(ERR, -1, NULL, msg);
    return err;
}

static token *read_next(lexer *l)
{
    skip_whitespace(l);
    token *result = NULL;
    char c = curr_char(l);
    if (c == '\0') {
        result = token_new(END, 0, NULL, NULL);
    } else if (isdigit(c)) {
        result = read_digit(l);
    } else if (isop(c)) {
        result = token_new(chartype(advance(l)), 0, NULL, NULL);
    } else if (isparen(c)) {
        result = token_new(chartype(advance(l)), 0, NULL, NULL);
    } else if (isbrace(c)) {
        result = token_new(chartype(advance(l)), 0, NULL, NULL);
    } else if (isalpha(c)) {
        result = read_ident(l);
    } else if (is_newline(c)) {
        result = token_new(chartype(advance(l)), 0, NULL, NULL);
    } else if (iscomma(c)) {
        return token_new(chartype(advance(l)), 0, NULL, NULL);
    } else if (iscomment(c)) {
        advance(l);
        skip_comment(l);
    } else {
        result = lexer_error(l, c);
        advance(l);
    }
    return result;
}

token *lexer_currtok(lexer* l)
{
    if (l->currtok == NULL)
        l->currtok = read_next(l);
    return l->currtok;
}

token *lexer_advance(lexer* l)
{
    token *tmp = lexer_currtok(l);
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
    toktype curr_type = token_gettype(l->currtok);
    return curr_type == END;
}

void lexer_halt(lexer* l)
{
    l->currtok = token_new(END, 0, NULL, NULL);
}

void lexer_delete(lexer *l)
{
    if (l) {
        if (l->input)
            free(l->input);
	free(l);
    }
}
