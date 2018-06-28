#include "parser.h"
#include "evaluator.h"
#include "symtable.h"
#include "tokenizer.h"
#include "error.h"

#include <string.h>

Token curr, lookahead;
int neg = 1;

void stmt();
void declaration();
void assignment();
void expr();
void term();
void factor();
void match(TokenType typ);

void parse(char *input)
{
    tokenizer_init(input);
    curr = lexan();
    lookahead = lexan();
    stmt();
    stack_print();
}

void stmt()
{
    if (strcmp(curr.val, "let") == 0)
        return declaration();
    else if (curr.type == tokenIDENT) {
        if (lookahead.type == tokenEQUAL)
            return assignment();
    }
    return expr();
}

void declaration()
{
    match(tokenIDENT);
    symtable_insert(curr.val);
    if (lookahead.type == tokenEQUAL)
        assignment();
    else stack_error("must assign variable upon declaration");
}

void assignment()
{
    StackEntry var;
    var.type = tokenIDENT;
    strcpy(var.ident, curr.val);
    match(tokenIDENT);
    stack_push(var);
    Token eq = curr;
    match(tokenEQUAL);
    expr();
    eval_binop(eq);
}

void expr()
{
    term();
    Token tok;
    while (curr.type == tokenPLUS ||
           curr.type == tokenMINUS ||
           curr.type == tokenPERCENT ||
           curr.type == tokenAMPERSAND ||
           curr.type == tokenPIPE
    ) {
        tok = curr;
        match(tok.type);
        term();
        eval_binop(tok);
    }
}

void term()
{
    factor();
    Token tok;
    while (curr.type == tokenSTAR ||
           curr.type == tokenSLASH ||
           curr.type == tokenDBLSTAR
    ) {
        tok = curr;
        match(tok.type);
        factor();
        eval_binop(tok);
    }
}

void factor()
{
    StackEntry e;
    Token tok;
    char *name;
    int idx;
    switch (curr.type) {
    case tokenPLUS:
    case tokenMINUS:
        tok = curr;
        if (curr.type == tokenMINUS)
            neg *= -1;
        match(tok.type);
        factor();
        break;
    case tokenINT:
        tok = curr;
        e.type = tok.type;
        e.ival = neg * atoi(tok.val);
        match(tokenINT);
        stack_push(e);
        neg = 1;
        break;
    case tokenFLOAT:
        tok = curr;
        e.type = tok.type;
        e.fval = neg * atof(tok.val);
        match(tokenFLOAT);
        stack_push(e);
        neg = 1;
        break;
    case tokenLPAREN:
        match(tokenLPAREN);
        expr();
        match(tokenRPAREN);
        break;
    case tokenIDENT:
        name = curr.val;
        idx = symtable_lookup(name);
        match(tokenIDENT);
        if (idx >= 0) {
            e.type = symtable[idx].type;
            switch (e.type) {
            case tokenINT:
                e.ival = symtable[idx].ival;
                break;
            case tokenFLOAT:
                e.fval = symtable[idx].fval;
                break;
            default:
                fatal("unknown data type for ident");
            }
            stack_push(e);
        } else stack_error("reference error");
        break;
    default:
        stack_error("bad factor syntax");
    }
}

void match(TokenType typ)
{
    if (curr.type == typ) {
        curr = lookahead;
        lookahead = lexan();
    }
    else fatal("whoops");
}