#include "parser.h"
#include "evaluator.h"
#include "tokenizer.h"
#include "error.h"

#include <string.h>

Token curr;

void expr(Tokenizer *t);
void term(Tokenizer *t);
void factor(Tokenizer *t);
void match(Tokenizer *t, TokenType typ);

void parse(char *input)
{
    Tokenizer *t = TokenizerInit(input);
    curr = lexan(t);
    expr(t);
    stack_print();
    TokenizerDelete(t);
}

void expr(Tokenizer *t)
{
    term(t);
    Token tok;
    while (curr.type == tokenPLUS || curr.type == tokenMINUS) {
        switch (curr.type) {
            case tokenPLUS:
            case tokenMINUS:
                tok = curr;
                match(t, tok.type);
                term(t);
                eval_binop(tok);
                break;
        }
    }
}

void term(Tokenizer *t)
{
    factor(t);
    Token tok;
    while (curr.type == tokenSTAR || curr.type == tokenSLASH) {
        switch (curr.type) {
            case tokenSTAR:
            case tokenSLASH:
                tok = curr;
                match(t, tok.type);
                factor(t);
                eval_binop(tok);
                break;
        }
    }
}

void factor(Tokenizer* t)
{
    StackEntry e;
    Token tok;
    switch (curr.type) {
    case tokenINT:
        tok = curr;
        match(t, tokenINT);
        e.type = tok.type;
        e.ival = atoi(tok.val);
        stack_push(e);
        break;
    case tokenFLOAT:
        tok = curr;
        match(t, tokenFLOAT);
        e.type = tok.type;
        e.fval = atof(tok.val);
        stack_push(e);
        break;
    case tokenBUILTIN:
        tok = curr;
        match(t, tokenBUILTIN);
        match(t, tokenLPAREN);
        expr(t);
        match(t, tokenRPAREN);
        eval_builtin(tok);
        break;
    case tokenLPAREN:
        match(t, tokenLPAREN);
        expr(t);
        match(t, tokenRPAREN);
        break;
    }
}

void match(Tokenizer *t, TokenType typ)
{
    if (curr.type == typ)
        curr = lexan(t);
    else fatal("whoops");
}