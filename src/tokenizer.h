#ifndef _LEXER_H
#define _LEXER_H

#define BSIZE 128

#include "token.h"

void tokenizer_init();
void tokenizer_run();
Token lexan();
void tokenizer_halt();

#endif