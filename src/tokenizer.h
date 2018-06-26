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

Tokenizer *tokenizer_init(char *input);

void tokenizer_run(Tokenizer *t);

Token lexan(Tokenizer *t);

void tokenizer_delete(Tokenizer *t);

#endif