#include "../headers/global.h"

int lookahead;

void parse(char *input)
{
    lookahead = lexan(input);
    expr(input);
}

void expr(char *input)
{
    int t;
    term(input);
    while(1) {
        switch (lookahead) {
        case '+': case '-':
            t = lookahead;
            match(input, lookahead); term(input); emit(t, NONE);
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
            match(input, lookahead); factor(input); emit(t, NONE);
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
    case NUM:
        emit(NUM, tokenval); match(input, NUM); break;
    default:
        printf("unexpected '%c'\n", lookahead);
        error("syntax error");
    }
}

void match(char *input, int t)
{
    if (lookahead == t)
        lookahead = lexan(input);
    else error("unexpected token error");
}