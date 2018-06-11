#include "headers/global.h"

int lookahead = NONE;
int neg = 1;

void parse(char *input)
{
    lookahead = lexan(input);
    expr(input);
    lexreset();
}

void expr(char *input)
{
    int t;
    term(input);
    while(1) {
        switch (lookahead) {
        case '+': case '-':
            t = lookahead;
            match(input, lookahead); term(input); eval(t);
            continue;
        default:
            return;
        }
    }
}

void term(char *input)
{
    int t;
    factor(input);
    while (1) {
        switch (lookahead) {
        case '*': case '/':
            t = lookahead;
            match(input, lookahead); factor(input); eval(t);
            continue;
        default:
            return;
        }
    }
}

void factor(char *input)
{
    switch (lookahead) {
    case '(':
        match(input, '('); expr(input); match(input, ')'); break;
    case '+': case '-':
        if (lookahead == '-')
            neg *= -1;
        match(input, lookahead);
        factor(input);
        break;
    case NUM:
        tokenval *= neg;
        eval(NUM); match(input, NUM);
        neg = 1;
        break;
    default:
        printf("unexpected '%c'\n", lookahead);
        error("syntax error");
    }
}

void match(char *input, int t)
{
    if (lookahead == t)
        lookahead = lexan(input);
    else {
        printf("unexpected token '%c'\n", lookahead);
        error("unexpected token error");
    }
}