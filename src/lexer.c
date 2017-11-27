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
    while (isspace(curr_char(l))) {
        advance(l);
    }
}

static int isop(char c)
{
    return c == '+' || c == '-' ||
        c == '*' || c == '/' ||
        c == '=' || c == '<' ||
        c == '>' || c == '!';
}

static int isparen(char c)
{
    return c == '(' || c == ')';
}

static int isbrace(char c)
{
    return c == '{' || c == '}';
}

static int isdot(char c)
{
    return c == '.';
}

static int iscomma(char c)
{
    return c == ',';
}

static int ispound(char c)
{
    return c == '#';
}

static int isquote(char c)
{
    return c == '"';
}

static int issemi(char c)
{
    return c == ';';
}

static token *error_token(lexer *l, char *s)
{
    char *msg = malloc(MAXBUFSIZE*sizeof(char));
    snprintf(msg, (size_t)MAXBUFSIZE, "Unexpected '%s' at (%d|%d)",
             s, lexer_getline(l), lexer_getcol(l));
    token *err = token_new(ERR, NULL, msg);
    return err;
}

static token *read_digit(lexer *l)
{
    char* num = malloc(MAXBUFSIZE*sizeof(char));
    char c = curr_char(l);
    int hasdot = 0;

    char tmp[2] = {c};
    strcat(num, tmp);
    advance(l);
    c = curr_char(l);

    while (isdigit(c) || isdot(c)) {
        if (c == '.') {
            if (hasdot) {
                char s[2] = {c};
                return error_token(l, s);
            }
            hasdot = 1;
        }
        char tmp[2] = {c};
        strcat(num, tmp);
        advance(l);
        c = curr_char(l);
    }

    return hasdot ? token_new(FLOAT, num, NULL) : token_new(INT, num, NULL);
}

static int boolean(char *str)
{
    const char *bools[] = { "true", "false" };
    int i;
    for (i = 0; i < 2; i++) {
        if (strcmp(bools[i], str) == 0)
            return 1;
    }
    return 0;
}

static int reserved(char *str)
{
    const char *rwords[] = { "if", "else", "while" };
    int i;
    for (i = 0; i < 3; i++) {
        if (strcmp(rwords[i], str) == 0)
            return 1;
    }
    return 0;
}

static token *read_reserved(char *str)
{
    if (strcmp(str, "if") == 0)
        return token_new(IF, NULL, NULL);
    else if (strcmp(str, "else") == 0)
        return token_new(ELSE, NULL, NULL);
    else
        return token_new(WHILE, NULL, NULL);
}

static token *read_ident(lexer *l)
{
    char *str = malloc(MAXBUFSIZE*sizeof(char));
    char c = curr_char(l);

    while (isalpha(c)) {
        c = advance(l);
        char tmp[2] = {c};
        strcat(str, tmp);
        c = curr_char(l);
    }
    str = realloc(str, strlen(str));

    if (boolean(str)) {
        return token_new(BOOL, str, NULL);
    } else if (reserved(str)) {
        return read_reserved(str);
    }

    return token_new(IDENT, str, NULL);
}

static token *read_str(lexer *l)
{
    char *str = malloc(MAXBUFSIZE*sizeof(char));
    advance(l); /* Ignore opening quote */
    char c = curr_char(l);

    int count = 0;
    while (c != '"' && count < MAXBUFSIZE) {
        char tmp[2] = {c};
        strcat(str, tmp);
        count++;
        advance(l);
        c = curr_char(l);
    }

    /* Check for closing quote and move beyond it*/
    if (c != '"') {
        return error_token(l, "eof");
    }
    advance(l);

    return token_new(STRING, str, NULL);
}

static token *op_token(lexer *l, char *op)
{
    toktype t;
    if (strcmp(op, "+") == 0)
        t = PLUS;
    else if (strcmp(op, "-") == 0)
        t = MINUS;
    else if (strcmp(op, "*") == 0)
        t = TIMES;
    else if (strcmp(op, "/") == 0)
        t = DIVIDE;
    else if (strcmp(op, "=") == 0)
        t = ASSIGN;
    else if (strcmp(op, "==") == 0)
        t = EQUALS;
    else if (strcmp(op, "!=") == 0)
        t = NEQUALS;
    else if (strcmp(op, "<") == 0)
        t = LT;
    else if (strcmp(op, "<=") == 0)
        t = LTE;
    else if (strcmp(op, ">") == 0)
        t = GT;
    else if (strcmp(op, ">=") == 0)
        t = GTE;
    else
        return error_token(l, op);

    return token_new(t, NULL, NULL);
}

static token *read_op(lexer *l)
{
    char *op = malloc(MAXBUFSIZE*sizeof(char));
    char c = curr_char(l);

    while (isop(c)) {
        advance(l);
        char tmp[2] = {c};
        strcat(op, tmp);
        c = curr_char(l);
    }
    op = realloc(op, strlen(op));

    return op_token(l, op);
}

static void skip_comment(lexer *l)
{
    advance(l);
    char c = curr_char(l);

    while (c != '\n') {
        advance(l);
        c = curr_char(l);
    }

    advance(l);
}

static token *paren_token(char c)
{
    toktype paren;
    switch(c) {
    case '(': paren = LPAREN; break;
    case ')': paren = RPAREN; break;
    }

    return token_new(paren, NULL, NULL);
}

static token* brace_token(char c)
{
    toktype brace;
    switch(c) {
    case '{': brace = LBRACE; break;
    case '}': brace = RBRACE; break;
    }

    return token_new(brace, NULL, NULL);
}

static token *read_next(lexer *l)
{
    skip_whitespace(l);
    token *result = NULL;
    char c = curr_char(l);
    if (c == '\0') {
        result = token_new(END, NULL, NULL);
    } else if (isdigit(c)) {
        result = read_digit(l);
    } else if (isop(c)) {
        result = read_op(l);
    } else if (isparen(c)) {
        result = paren_token(advance(l));
    } else if (isbrace(c)) {
        result = brace_token(advance(l));
    } else if (isalpha(c)) {
        result = read_ident(l);
    } else if (iscomma(c)) {
        result = token_new(COMMA, NULL, NULL);
        advance(l);
    } else if (isquote(c)) {
        result = read_str(l);
    } else if (ispound(c)) {
        skip_comment(l);
        result = read_next(l);
    } else if (issemi(c)) {
        result = token_new(SEMI, NULL, NULL);
        advance(l);
    } else {
        char s[2] = {c};
        result = error_token(l, s);
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
    l->currtok = token_new(END, NULL, NULL);
}

void lexer_delete(lexer *l)
{
    if (l) {
        if (l->input)
            free(l->input);
        free(l);
    }
}
