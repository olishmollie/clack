#include "tokenizer.h"
#include "error.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Token lexan(Tokenizer *t);
void skipWhitespace(Tokenizer *t);
Token lexDigit(Tokenizer *t);
Token lexIdent(Tokenizer *t);
Token lexPunct(Tokenizer *t);
Token produce(Tokenizer *t, TokenType type);
int accept(Tokenizer *t, char *valid);
void acceptRun(Tokenizer *t, char *valid);
char peek(Tokenizer *t);
char next(Tokenizer *t);
void ignore(Tokenizer *t);
void backup(Tokenizer *t);
Token lexError(Tokenizer *t, char *msg);

Tokenizer *TokenizerInit(char *input)
{
    Tokenizer *t = malloc(sizeof(Tokenizer));
    if (t == NULL) {
        fatal("out of memory");
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
        TokenDelete(tok);
        tok = lexan(t);
    }
}

Token lexan(Tokenizer *t)
{
    skipWhitespace(t);
    char c = next(t);

    if (c < 0)
        return NewToken(tokenEOF, "NONE");
    else if (isdigit(c))
        return lexDigit(t);
    else if (isalnum(c))
        return lexIdent(t);
    else if (ispunct(c))
        return lexPunct(t);

    return lexError(t, "illegal token");
}

void skipWhitespace(Tokenizer *t)
{
    while (isspace(t->input[t->pos])) {
        t->pos++;
        ignore(t);
    }
}

Token lexDigit(Tokenizer *t)
{
    int frac = 0, rat = 0;
    char *digits = "0123456789";
    acceptRun(t, digits);
    if (accept(t, "/")) {
        rat = 1;
        acceptRun(t, digits);
    } else if (accept(t, ".")) {
        frac = 1;
        acceptRun(t, digits);
    }
    if (isalpha(peek(t))) {
        return lexError(t, "bad number syntax");
    }

    TokenType typ;
    if (rat) typ = tokenRAT;
    else if (frac) typ = tokenFLOAT;
    else typ = tokenINT;

    return produce(t, typ);
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
    else res = produce(t, tokenIDENT);
    free(cur);
    return res;
}

Token lexPunct(Tokenizer *t)
{
    backup(t);
    char c = next(t);
    switch (c) {
    case '+':
        return produce(t, tokenPLUS);
    case '-':
        return produce(t, tokenMINUS);
    case '/':
        return produce(t, tokenSLASH);
    case '&':
        return produce(t, tokenAMPERSAND);
    case '(':
        return produce(t, tokenLPAREN);
    case ')':
        return produce(t, tokenRPAREN);
    case '|':
        return produce(t, tokenPIPE);
    case '*':
        if (peek(t) == '*') {
            next(t);
            return produce(t, tokenDBLSTAR);
        }
        return produce(t, tokenSTAR);
    case '<':
        if (peek(t) == '<') {
            next(t);
            return produce(t, tokenLSHIFT);
        }
        return produce(t, tokenLCARAT);
    }
    return lexError(t, "unknown punctuation");
}

Token produce(Tokenizer *t, TokenType type)
{
    char *val = currVal(t);
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

void ignore(Tokenizer *t)
{
    t->start = t->pos;
}

void backup(Tokenizer *t)
{
    t->pos--;
}

Token lexError(Tokenizer *t, char *msg)
{
    int len = strlen(msg);
    char *m = malloc(sizeof(char)*len+1);
    strcpy(m, msg);
    m[len] = '\0';
    next(t);
    t->start = t->pos;
    return NewToken(tokenERROR, m);
}

void TokenizerDelete(Tokenizer *t)
{
    free(t->input);
    free(t);
}