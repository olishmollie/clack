#ifndef _PARSER_H
#define _PARSER_H

#include "lexer.h"

class Parser {
    Lexer lexer;
    Token curr;
public:
    Parser(const char*);
    int parse();
    int expr();
    int term();
    int factor();
    void match(int);
};

#endif