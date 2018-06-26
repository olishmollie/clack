#include "global.h"
#include "evaluator.h"
#include "error.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

StackEntry stack[STACKSIZE];
int top = -1;

/* ********** BUILTINS ********* */
int eval_ln();
int eval_log();
int eval_ans();

int eval_builtin(Token builtin)
{
    int res = 0;
    if (strcmp(builtin.val, "ln") == 0) {
        token_delete(builtin);
        res = eval_ln();
    }
    else if (strcmp(builtin.val, "log") == 0) {
        token_delete(builtin);
        res = eval_log();
    }
    else if (strcmp(builtin.val, "ans") == 0) {
        token_delete(builtin);
        res = eval_ans();
    }
    return res;
}

int eval_ln()
{
    StackEntry e;
    StackEntry arg = stack[top];
    e.type = tokenFLOAT;
    if (arg.type == tokenINT) {
        e.fval = log(arg.ival);
    } else {
        e.fval = log(arg.fval);
    }
    stack[top] = e;
    return 1;
}

int eval_log()
{
    StackEntry e;
    StackEntry arg = stack[top];
    e.type = tokenFLOAT;
    if (arg.type == tokenINT) {
        e.fval = log10(arg.ival);
    } else {
        e.fval = log(arg.fval);
    }
    stack[top] = e;
    return 1;
}

int eval_ans()
{
    StackEntry arg = stack[top];
    int numEntries = top+1;
    if (arg.type != tokenINT || arg.ival >= numEntries || arg.ival <= 0) {
        fatal("invalid ans argument");
    }
    StackEntry e;
    int offset = top - arg.ival;
    printf("offset = %d\n", offset);
    e.type = stack[offset].type;
    switch (e.type) {
    case tokenINT:
        e.ival = stack[offset].ival;
        break;
    case tokenFLOAT:
        e.fval = stack[offset].fval;
        break;
    default:
        return 0;
    }
    stack[top] = e;
    return 1;
}

/* ********** BINOPS ********** */
int eval_add(StackEntry left, StackEntry right);
int eval_sub(StackEntry left, StackEntry right);
int eval_mul(StackEntry left, StackEntry right);
int eval_div(StackEntry left, StackEntry right);

int eval_binop(Token op)
{
    StackEntry right = stack[top];
    stack_pop();
    StackEntry left = stack[top];
    switch (op.type) {
        case tokenPLUS:
            eval_add(left, right);
            break;
        case tokenMINUS:
            eval_sub(left, right);
        case tokenSTAR:
            eval_mul(left, right);
            break;
        case tokenSLASH:
            eval_div(left, right);
            break;
        default:
            return 0;
    }
    return 1;
}

int eval_add(StackEntry left, StackEntry right)
{
    StackEntry e;
    int frac = (left.type == tokenFLOAT || right.type == tokenFLOAT);
    if (frac) {
        double l, r;
        if (left.type != tokenFLOAT) {
            l = left.ival;
        } else {
            l = left.fval;
        }

        if (right.type != tokenFLOAT) {
            r = right.ival;
        } else {
            r = right.fval;
        }

        e.type = tokenFLOAT;
        e.fval = l + r;
        stack[top] = e;
    } else {
        e.type = tokenINT;
        e.ival = left.ival + right.ival;
        stack[top] = e;
    }
    return 1;
}

int eval_sub(StackEntry left, StackEntry right)
{
    StackEntry e;
    int frac = (left.type == tokenFLOAT || right.type == tokenFLOAT);
    if (frac) {
        double l, r;
        if (left.type != tokenFLOAT) {
            l = left.ival;
        } else {
            l = left.fval;
        }

        if (right.type != tokenFLOAT) {
            r = right.ival;
        } else {
            r = right.fval;
        }

        e.type = tokenFLOAT;
        e.fval = l - r;
        stack[top] = e;
    } else {
        e.type = tokenINT;
        e.ival = left.ival - right.ival;
        stack[top] = e;
    }
    return 1;
}

int eval_mul(StackEntry left, StackEntry right)
{
    StackEntry e;
    int frac = (left.type == tokenFLOAT || right.type == tokenFLOAT);
    if (frac) {
        double l, r;
        if (left.type != tokenFLOAT) {
            l = left.ival;
        } else {
            l = left.fval;
        }

        if (right.type != tokenFLOAT) {
            r = right.ival;
        } else {
            r = right.fval;
        }

        e.type = tokenFLOAT;
        e.fval = l * r;
        stack[top] = e;
    } else {
        e.type = tokenINT;
        e.ival = left.ival * right.ival;
        stack[top] = e;
    }
    return 1;
}

int eval_div(StackEntry left, StackEntry right)
{
    StackEntry e;
    int frac = (left.type == tokenFLOAT || right.type == tokenFLOAT);
    if (frac) {
        double l, r;
        if (left.type != tokenFLOAT) {
            l = left.ival;
        } else {
            l = left.fval;
        }

        if (right.type != tokenFLOAT) {
            if (right.ival == 0) {
                e.type = tokenERROR;
                e.err = "division by zero";
                stack_push(e);
                return 0;
            }
            r = right.ival;
        } else {
            if (right.fval == 0) {
                e.type = tokenERROR;
                e.err = "division by zero";
                stack_push(e);
                return 0;
            }
            r = right.fval;
        }

        e.type = tokenFLOAT;
        e.fval = l / r;
        stack[top] = e;
    } else {
        e.type = tokenINT;
        if (right.ival == 0) {
            e.type = tokenERROR;
            e.err = "division by zero";
            stack_push(e);
            return 0;
        }
        e.ival = left.ival / right.ival;
        stack[top] = e;
    }
    return 1;
}

/* ********** STACK OPS ********** */
int stack_push(StackEntry e)
{
    if (top < STACKSIZE-1) {
        stack[++top] = e;
        return top;
    }
    return -1;
}

int stack_pop()
{
    if (top >= 0) {
        return --top;
    }
    return -1;
}

void stack_clear()
{
    top = -1;
}

void stack_print()
{
    int numEl = top + 1;
    for (int i = 0; i < numEl; i++) {
        char *arrow = (i == top) ? "<-" : "";
        switch (stack[i].type) {
        case tokenINT:
            printf("%d: %d %s\n", i+1, stack[i].ival, arrow);
            break;
        case tokenFLOAT:
            printf("%d: %f %s\n", i+1, stack[i].fval, arrow);
            break;
        case tokenIDENT:
            printf("%d: %s %s\n", i+1, stack[i].ident, arrow);
            break;
        case tokenERROR:
            printf("%d: %s %s\n", i+1, stack[i].err, arrow);
        default:
            ;
        }
    }
}

void stack_delete()
{
    int numEntries = top + 1;
    for (int i = 0; i < numEntries; i++) {
        if (stack[i].type == tokenIDENT)
            free(stack[i].ident);
    }
}