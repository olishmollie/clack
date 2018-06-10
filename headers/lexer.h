#ifndef _LEXER_H
#define _LEXER_H

#include <ctype.h>

#include "token.h"

class Lexer {
    int lookahead;
    const char *input;
public:
    Lexer() : lookahead(0), input(nullptr) {}
    Lexer(const char *in) : lookahead(0), input(in) {}
    Token lex();
};

#endif