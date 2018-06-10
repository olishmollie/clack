#include "../headers/lexer.h"

#include <iostream>

Token Lexer::lex()
{
    char t = input[lookahead];

    while (isspace(t)) {
        t = input[lookahead++];
    }

    if (isdigit(t)) {
        int v = 0;
        while (isdigit(t)) {
            int tmp = v * 10 + (t-'0');
            t = input[++lookahead];
        }
        return Token(NUM, v);
    }
    else if (t == '\0') {
        ++lookahead;
        return Token(DONE, -1);
    }
    else {
        ++lookahead;
        return Token(t, -1);
    }
}

