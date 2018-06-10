#include "../headers/parser.h"

#include <iostream>

Parser::Parser(const char *in)
{
    lexer = Lexer(in);
    curr = lexer.lex();
}

int Parser::parse()
{
    return expr();
}

int Parser::expr()
{
    int lval = term(), rval;
    while (true) {
        switch (curr.type) {
            case '+':
                match(curr.type);
                rval = term();
                return lval + rval;
            case '-':
                match(curr.type);
                rval = term();
                return lval - rval;
            default:
                return lval;
        }
    }
}

int Parser::term()
{
    int lval = factor(), rval;
    while (true) {
        switch (curr.type) {
            case '*': 
                match(curr.type);
                rval = factor();
                return lval * rval;
            case '/':
                match(curr.type);
                rval = factor();
                if (rval == 0)
                    throw "division by zero";
                return lval / rval;
            default:
                return lval;
        }
    }
}

int Parser::factor()
{
    int res;
    switch (curr.type) {
        case '(':
            match('('); res = expr(); match(')');
            return res;
        case NUM:
            res = curr.value - '0';
            match(NUM);
            return res;
        default:
            throw "syntax error";
    }
}

void Parser::match(int c)
{
    if (c == curr.type) {
        curr = lexer.lex();
    } else {
        throw "unexpected token";
    }
}