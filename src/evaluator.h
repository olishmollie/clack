#ifndef _STACk_H
#define _STACk_H

#include "token.h"

#define STACKSIZE 20

typedef struct StackEntry {
    TokenType type;
    union {
        int ival;
        double fval;
        char ident[MAXLEN];
        char *err;
    };
} StackEntry;

StackEntry stack[STACKSIZE];
int top;

int eval_binop(Token op);
int stack_push(StackEntry e);
int stack_pop();
void stack_print();

void stack_clear();
int stack_error();

#endif