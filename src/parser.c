#include "global.h"
#include "parser.h"
#include "evaluator.h"
#include "tokenizer.h"
#include "error.h"

#include <string.h>

Token curr;
int neg = 1;

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
    while (curr.type == tokenPLUS ||
           curr.type == tokenMINUS ||
           curr.type == tokenPERCENT ||
           curr.type == tokenAMPERSAND ||
           curr.type == tokenPIPE
    ) {
        tok = curr;
        match(t, tok.type);
        term(t);
        eval_binop(tok);
    }
}

void term(Tokenizer *t)
{
    factor(t);
    Token tok;
    while (curr.type == tokenSTAR ||
           curr.type == tokenSLASH ||
           curr.type == tokenDBLSTAR
    ) {
        tok = curr;
        match(t, tok.type);
        factor(t);
        eval_binop(tok);
    }
}

void factor(Tokenizer* t)
{
    StackEntry e;
    Token tok;
    switch (curr.type) {
    case tokenPLUS:
    case tokenMINUS:
        tok = curr;
        if (curr.type == tokenMINUS)
            neg *= -1;
        match(t, tok.type);
        factor(t);
        break;
    case tokenINT:
        tok = curr;
        e.type = tok.type;
        e.ival = neg * atoi(tok.val);
        match(t, tokenINT);
        stack_push(e);
        token_delete(tok);
        neg = 1;
        break;
    case tokenFLOAT:
        tok = curr;
        e.type = tok.type;
        e.fval = neg * atof(tok.val);
        match(t, tokenFLOAT);
        stack_push(e);
        token_delete(tok);
        neg = 1;
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