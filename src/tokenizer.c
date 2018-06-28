#include "tokenizer.h"
#include "error.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Token lexan();
void skipWhitespace();
Token lexDigit();
Token lexIdent();
Token lexPunct();
Token produce(TokenType type);
int accept(char *valid);
void acceptRun(char *valid);
char peek();
char next();
void ignore();
void backup();
Token lexError(char *msg);

char input[BSIZE];
int start;
int pos;

void tokenizer_init(char *in)
{
    int len = strlen(in);
    if (len >= BSIZE)
        fatal("input too long");
    strncpy(input, in, len);
    input[len] = '\0';
    start = pos = 0;
}

void tokenizer_run()
{
    Token tok = lexan();
    while (tok.type != tokenEOF) {
        token_print(tok);
        tok = lexan();
    }
}

Token lexan()
{
    skipWhitespace();
    char c = next();

    if (c < 0)
        return token_new(tokenEOF, "NONE", 4);
    else if (isdigit(c))
        return lexDigit();
    else if (isalnum(c))
        return lexIdent();
    else if (ispunct(c))
        return lexPunct();

    return lexError("illegal token");
}

void skipWhitespace()
{
    while (isspace(input[pos])) {
        ignore();
    }
}

Token lexDigit()
{
    int frac = 0, rat = 0;
    char *digits = "0123456789";
    acceptRun(digits);
    if (accept("/")) {
        rat = 1;
        acceptRun(digits);
    } else if (accept(".")) {
        frac = 1;
        acceptRun(digits);
    }
    if (isalpha(peek())) {
        return lexError("bad number syntax");
    }

    TokenType typ;
    if (rat) typ = tokenRAT;
    else if (frac) typ = tokenFLOAT;
    else typ = tokenINT;

    return produce(typ);
}

Token lexIdent()
{
    char *alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    acceptRun(alphabet);
    return produce(tokenIDENT);
}

Token lexPunct()
{
    backup();
    char c = next();
    switch (c) {
    case '+':
        return produce(tokenPLUS);
    case '-':
        return produce(tokenMINUS);
    case '/':
        return produce(tokenSLASH);
    case '&':
        return produce(tokenAMPERSAND);
    case '(':
        return produce(tokenLPAREN);
    case ')':
        return produce(tokenRPAREN);
    case '|':
        return produce(tokenPIPE);
    case '^':
        return produce(tokenUPCARAT);
    case '%':
        return produce(tokenPERCENT);
    case '=':
        return produce(tokenEQUAL);
    case '*':
        if (peek() == '*') {
            next();
            return produce(tokenDBLSTAR);
        }
        return produce(tokenSTAR);
    case '<':
        if (peek() == '<') {
            next();
            return produce(tokenLSHIFT);
        }
        return produce(tokenLCARAT);
    }
    return lexError("unknown punctuation");
}

Token produce(TokenType type)
{
    int len = pos - start;
    Token tok = token_new(type, &input[start], len);
    start = pos;
    return tok;
}

int accept(char *valid)
{
    if (strchr(valid, next())) {
        return 1;
    }
    backup();
    return 0;
}

void acceptRun(char *valid)
{
    while (strchr(valid, next())) {
        ;
    }
    backup();
}

char peek()
{
    char c = next();
    backup();
    return c;
}

char next()
{
    char next = input[pos++];
    if (next) {
        return next;
    }
    return -1;
}

void ignore()
{
    start = ++pos;
}

void backup()
{
    pos--;
}

Token lexError(char *msg)
{
    int inputlen = strlen(msg);
    next();
    start = pos;
    return token_new(tokenERROR, msg, inputlen);
}
