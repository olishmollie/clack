#include "parser.h"
#include "evaluator.h"
#include "symtable.h"
#include "tokenizer.h"
#include "error.h"

#include <string.h>

Token curr;
int neg = 1;

void stmt(Tokenizer *t);
void assignment(Tokenizer *t);
void expr(Tokenizer *t);
void term(Tokenizer *t);
void factor(Tokenizer *t);
void match(Tokenizer *t, TokenType typ);

void parse(char *input)
{
    Tokenizer *t = tokenizer_init(input);
    curr = lexan(t);
    stmt(t);
    stack_print();
    tokenizer_delete(t);
}

void stmt(Tokenizer *t)
{
    if (strcmp(curr.val, "let") == 0) {
        return assignment(t);
    }
    return expr(t);
}

void assignment(Tokenizer *t)
{
    StackEntry var;
    match(t, tokenIDENT); /* let token */
    var.type = tokenIDENT;
    strcpy(var.ident, curr.val);
    symtable_insert(curr.val);
    match(t, tokenIDENT);
    stack_push(var);
    Token eq = curr;
    match(t, tokenEQUAL);
    expr(t);
    eval_binop(eq);
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
    char *name;
    int idx;
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
        neg = 1;
        break;
    case tokenFLOAT:
        tok = curr;
        e.type = tok.type;
        e.fval = neg * atof(tok.val);
        match(t, tokenFLOAT);
        stack_push(e);
        neg = 1;
        break;
    case tokenLPAREN:
        match(t, tokenLPAREN);
        expr(t);
        match(t, tokenRPAREN);
        break;
    case tokenIDENT:
        name = curr.val;
        idx = symtable_lookup(name);
        match(t, tokenIDENT);
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
        } else fatal("reference error");
        break;
    default:
        stack_error("bad factor syntax");
    }
}

void match(Tokenizer *t, TokenType typ)
{
    if (curr.type == typ)
        curr = lexan(t);
    else fatal("whoops");
}