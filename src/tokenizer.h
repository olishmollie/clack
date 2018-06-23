#ifndef _LEXER_H
#define _LEXER_H

#define BSIZE 128

#include "token.h"

typedef struct Tokenizer {
    char *input;
    int len;
    int start;
    int pos;
} Tokenizer;

Tokenizer *TokenizerInit(char *input);

void TokenizerRun(Tokenizer *t);

Token lexan(Tokenizer *t);

void TokenizerDelete(Tokenizer *t);

#endif