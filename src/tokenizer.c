#include "tokenizer.h"
#include "token.h"
#include "error.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Token lexan(Tokenizer *t);
Token lexDigit(Tokenizer *t);
Token lexIdent(Tokenizer *t);
Token produce(Tokenizer *t, TokenType type);
int accept(Tokenizer *t, char *valid);
void acceptRun(Tokenizer *t, char *valid);
char peek(Tokenizer *t);
char next(Tokenizer *t);
void backup(Tokenizer *t);

Tokenizer *TokenizerInit(char *input)
{
    Tokenizer *t = malloc(sizeof(Tokenizer));
    if (t == NULL) {
        error("out of memory");
    }
    t->start = t->pos = 0;
    t->input = input;
    t->len = strlen(input);
    return t;
}

void TokenizerRun(Tokenizer *t)
{
    Token tok = lexan(t);
    while (tok.type != tokenEOF) {
        TokenPrint(tok);
        tok = lexan(t);
    }
}

Token lexan(Tokenizer *t)
{
    char c = next(t);
    while (isspace(c)) {
        c = next(t);
    }
    backup(t);

    if (c < 0)
        return NewToken(tokenEOF, "NONE");
    else if (accept(t, "+-") || isdigit(c))
        return lexDigit(t);
    else if (isalnum(c)) {
        return lexIdent(t);
    }

    return NewToken(tokenERROR, "got to end of lexer");
}

Token lexDigit(Tokenizer *t)
{
    int frac = 0;
    char *digits = "0123456789";
    acceptRun(t, digits);
    if (accept(t, ".")) {
        frac = 1;
        acceptRun(t, digits);
    }
    return produce(t, frac ? tokenFLOAT : tokenINT);
}

char *currVal(Tokenizer *t)
{
    int len = t->pos - t->start;
    char *val = malloc(sizeof(char)*len+1);
    strncpy(val, &t->input[t->start], len);
    val[len] = '\0';
    return val;
}

Token lexIdent(Tokenizer *t)
{
    char *alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    acceptRun(t, alphabet);
    char *cur = currVal(t);
    Token res;
    if (strcmp(cur, "log") == 0)
        res = produce(t, tokenLOG);
    else if (strcmp(cur, "ln") == 0)
        res = produce(t, tokenLN);
    else if (strcmp(cur, "pow") == 0)
        res = produce(t, tokenPOW);
    else res = produce(t, tokenIDENT);
    free(cur);
    return res;
}

Token produce(Tokenizer *t, TokenType type)
{
    int len = t->pos - t->start;
    char val[len+1];
    strncpy(val, &t->input[t->start], len);
    val[len] = '\0';
    t->start = t->pos;
    return NewToken(type, val);
}

int accept(Tokenizer *t, char *valid)
{
    if (strchr(valid, next(t))) {
        return 1;
    }
    backup(t);
    return 0;
}

void acceptRun(Tokenizer *t, char *valid)
{
    while (strchr(valid, next(t))) {
        ;
    }
    backup(t);
}

char peek(Tokenizer *t)
{
    char c = next(t);
    backup(t);
    return c;
}

char next(Tokenizer *t)
{
    char next = t->input[t->pos++];
    if (next) {
        return next;
    }
    return -1;
}

void backup(Tokenizer *t)
{
    t->pos--;
}

void TokenizerDelete(Tokenizer *t)
{
    free(t->input);
    free(t);
}