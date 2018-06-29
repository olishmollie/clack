#include "evaluator.h"
#include "error.h"
#include "symtable.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

int top = -1;

/* ********** BINOPS ********** */
int eval_add(StackEntry left, StackEntry right);
int eval_sub(StackEntry left, StackEntry right);
int eval_mul(StackEntry left, StackEntry right);
int eval_div(StackEntry left, StackEntry right);
int eval_pow(StackEntry left, StackEntry right);

int eval_mod(StackEntry left, StackEntry right);
int eval_bwand(StackEntry left, StackEntry right);
int eval_bwor(StackEntry left, StackEntry right);
int eval_bwxor(StackEntry left, StackEntry right);

int eval_assign(StackEntry left, StackEntry right);

int eval_binop(Token op)
{
    StackEntry right = stack[top];
    stack_pop();
    StackEntry left = stack[top];
    switch (op.type) {
        case tokenPLUS:
            return eval_add(left, right);
        case tokenMINUS:
            return eval_sub(left, right);
        case tokenSTAR:
            return eval_mul(left, right);
        case tokenSLASH:
            return eval_div(left, right);
        case tokenPERCENT:
            return eval_mod(left, right);
        case tokenAMPERSAND:
            return eval_bwand(left, right);
        case tokenPIPE:
            return eval_bwor(left, right);
        case tokenUPCARAT:
            return eval_bwxor(left, right);
        case tokenDBLSTAR:
            return eval_pow(left, right);
        case tokenEQUAL:
            return eval_assign(left, right);
        default:
            return 0;
    }
}

int eval_add(StackEntry left, StackEntry right)
{
    StackEntry e;
    if (left.type == tokenFLOAT || right.type == tokenFLOAT) {
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
    } else if (left.type == tokenRAT || right.type == tokenRAT) {
        if (left.type == tokenRAT && right.type == tokenRAT) {
            rational r = rat_add(left.rval, right.rval);
            e.type = tokenRAT;
            e.rval = r;
            stack[top] = e;
        } else if (left.type == tokenRAT) {
            e.type = tokenFLOAT;
            switch (right.type) {
            case tokenINT:
                e.fval = to_double(left.rval) + right.ival;
                break;
            case tokenFLOAT:
                e.fval = to_double(left.rval) + right.fval;
                break;
            default:
                stack_errorf("unknown token type for right operand %d\n", right.type);
            }
            stack[top] = e;
        } else {
            e.type = tokenFLOAT;
            switch (left.type) {
            case tokenINT:
                e.fval = left.ival + to_double(right.rval);
                break;
            case tokenFLOAT:
                e.fval = left.fval + to_double(right.rval);
                break;
            default:
                stack_errorf("unknown token type for left operand %d\n", left.type);
            }
            stack[top] = e;
        }
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
    if (left.type == tokenFLOAT || right.type == tokenFLOAT) {
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
    } else if (left.type == tokenRAT || right.type == tokenRAT) {
        if (left.type == tokenRAT && right.type == tokenRAT) {
            rational r = rat_sub(left.rval, right.rval);
            e.type = tokenRAT;
            e.rval = r;
            stack[top] = e;
        } else if (left.type == tokenRAT) {
            e.type = tokenFLOAT;
            switch (right.type) {
            case tokenINT:
                e.fval = to_double(left.rval) - right.ival;
                break;
            case tokenFLOAT:
                e.fval = to_double(left.rval) - right.fval;
                break;
            default:
                stack_errorf("unknown token type for right operand %d\n", right.type);
            }
            stack[top] = e;
        } else {
            e.type = tokenFLOAT;
            switch (left.type) {
            case tokenINT:
                e.fval = left.ival - to_double(right.rval);
                break;
            case tokenFLOAT:
                e.fval = left.fval - to_double(right.rval);
                break;
            default:
                stack_errorf("unknown token type for left operand %d\n", left.type);
            }
            stack[top] = e;
        }
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
    if (left.type == tokenFLOAT || right.type == tokenFLOAT) {
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
    } else if (left.type == tokenRAT || right.type == tokenRAT) {
        if (left.type == tokenRAT && right.type == tokenRAT) {
            rational r = rat_mul(left.rval, right.rval);
            e.type = tokenRAT;
            e.rval = r;
            stack[top] = e;
        } else if (left.type == tokenRAT) {
            e.type = tokenFLOAT;
            switch (right.type) {
            case tokenINT:
                e.fval = to_double(left.rval) * right.ival;
                break;
            case tokenFLOAT:
                e.fval = to_double(left.rval) * right.fval;
                break;
            default:
                stack_errorf("unknown token type for right operand %d\n", right.type);
            }
            stack[top] = e;
        } else {
            e.type = tokenFLOAT;
            switch (left.type) {
            case tokenINT:
                e.fval = left.ival * to_double(right.rval);
                break;
            case tokenFLOAT:
                e.fval = left.fval * to_double(right.rval);
                break;
            default:
                stack_errorf("unknown token type for left operand %d\n", left.type);
            }
            stack[top] = e;
        }
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
    if (left.type == tokenFLOAT || right.type == tokenFLOAT) {
        double l, r;
        if (left.type != tokenFLOAT) {
            l = left.ival;
        } else {
            l = left.fval;
        }

        if (right.type != tokenFLOAT) {
            if (right.ival == 0)
                return stack_errorf("division by zero\n");
            r = right.ival;
        } else {
            if (right.fval == 0)
                return stack_errorf("division by zero\n");
            r = right.fval;
        }

        e.type = tokenFLOAT;
        e.fval = l / r;
        stack[top] = e;
    } else if (left.type == tokenRAT || right.type == tokenRAT) {
        if (left.type == tokenRAT && right.type == tokenRAT) {
            rational r = rat_div(left.rval, right.rval);
            e.type = tokenRAT;
            e.rval = r;
            stack[top] = e;
        } else if (left.type == tokenRAT) {
            e.type = tokenFLOAT;
            switch (right.type) {
            case tokenINT:
                e.fval = to_double(left.rval) / right.ival;
                break;
            case tokenFLOAT:
                e.fval = to_double(left.rval) / right.fval;
                break;
            default:
                stack_errorf("unknown token type for right operand %d\n", right.type);
            }
            stack[top] = e;
        } else {
            e.type = tokenFLOAT;
            switch (left.type) {
            case tokenINT:
                e.fval = left.ival / to_double(right.rval);
                break;
            case tokenFLOAT:
                e.fval = left.fval / to_double(right.rval);
                break;
            default:
                stack_errorf("unknown token type for left operand %d\n", left.type);
            }
            stack[top] = e;
        }
    } else {
        e.type = tokenINT;
        if (right.ival == 0)
            return stack_errorf("division by zero\n");
        e.ival = left.ival / right.ival;
        stack[top] = e;
    }
    return 1;
}

int eval_mod(StackEntry left, StackEntry right)
{
    StackEntry res;
    if (left.type == tokenINT && right.type == tokenINT) {
        if (right.ival == 0)
            return stack_errorf("division by zero\n");
        res.type = tokenINT;
        res.ival = left.ival % right.ival;
        stack[top] = res;
        return 1;
    }
    return stack_errorf("invalid arguments for modulo\n");
}

int eval_bwand(StackEntry left, StackEntry right)
{
    StackEntry res;
    if (left.type == tokenINT && right.type == tokenINT) {
        res.type = tokenINT;
        res.ival = left.ival & right.ival;
        stack[top] = res;
        return 1;
    }
    return stack_errorf("invalid arguments for bitwise and\n");
}

int eval_bwor(StackEntry left, StackEntry right)
{
    StackEntry res;
    if (left.type == tokenINT && right.type == tokenINT) {
        res.type = tokenINT;
        res.ival = left.ival | right.ival;
        stack[top] = res;
        return 1;
    }
    return stack_errorf("invalid arguments for bitwise or\n");
}

int eval_bwxor(StackEntry left, StackEntry right)
{
    StackEntry res;
    if (left.type == tokenINT && right.type == tokenINT) {
        res.type = tokenINT;
        res.ival = left.ival ^ right.ival;
        stack[top] = res;
        return 1;
    }
    return stack_errorf("invalid arguments for bitwise xor\n");
}

int eval_pow(StackEntry left, StackEntry right)
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
        e.fval = pow(l, r);
        stack[top] = e;
    } else {
        e.type = tokenINT;
        e.ival = (int)pow(left.ival, right.ival);
        stack[top] = e;
    }
    return 1;
}

int eval_assign(StackEntry left, StackEntry right)
{
    StackEntry e;
    if (left.type != tokenIDENT)
        return stack_errorf("invalid use of assignment\n");
    int idx = symtable_lookup(left.ident);
    symtable[idx].type = right.type;
    switch (right.type) {
    case tokenINT:
        symtable[idx].ival = right.ival;
        break;
    case tokenFLOAT:
        symtable[idx].fval = right.fval;
        break;
    case tokenRAT:
        symtable[idx].rval = right.rval;
        break;
    default:
        return stack_errorf("unknown data type for assignment\n");
    }
    stack_pop();
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

void stack_print()
{
    int numEl = top + 1;
    for (int i = 0; i < numEl; i++) {
        char *arrow = (i == top) ? "<-" : "";
        switch (stack[i].type) {
        case tokenINT:
            printf("%d: %d %s\n", i, stack[i].ival, arrow);
            break;
        case tokenFLOAT:
            printf("%d: %f %s\n", i, stack[i].fval, arrow);
            break;
        case tokenIDENT:
            printf("%d: %s %s\n", i, stack[i].ident, arrow);
            break;
        case tokenRAT:
            printf("%d: %d/%d %s\n", i, stack[i].rval.numer, stack[i].rval.denom, arrow);
            break;
        default:
            ;
        }
    }
}

void stack_clear()
{
    top = -1;
}

int stack_errorf(char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    stack_clear();
    va_end(args);
    return 0;
}