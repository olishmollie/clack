#ifndef _STACK_H
#define _STACK_H

#include "token.h"
#include "rational.h"

#define STACKSIZE 20

typedef struct StackEntry {
    TokenType type;
    union {
        int ival;
        double fval;
        rational rval;
        char ident[MAXLEN];
    };
} StackEntry;

StackEntry stack[STACKSIZE];
int top;

int eval_binop(Token op);
int stack_push(StackEntry e);
int stack_pop();
void stack_print();

void stack_clear();
int stack_errorf(char *fmt, ...);

#endif