#include "../headers/lexer.h"

#include <iostream>

Token Lexer::lex()
{
    char t = input[lookahead++];

    if (isdigit(t)) {
        return Token(NUM, t);
    }
    else if (t == '\0') {
        return Token(DONE, -1);
    }
    else {
        return Token(t, -1);
    }
}

