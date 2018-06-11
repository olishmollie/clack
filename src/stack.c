#include "headers/global.h"

int stack[MAXSIZE];
int ip = -1;

void stack_print()
{
    printf("%d\n", stack[ip]);
}

void push(int k)
{
    stack[++ip] = k;
}

void pop()
{
    if (ip >= 0) --ip;
    else error("cannot pop from empty stack");
}

void iadd() {
    int top = stack[ip];
    pop();
    stack[ip] = stack[ip] + top;
}

void isub() {
    int top = stack[ip];
    pop();
    stack[ip] = stack[ip] - top;
}

void imul() {
    int top = stack[ip];
    pop();
    stack[ip] = stack[ip] * top;
}

void idiv() {
    int top = stack[ip];
    if (top != 0) {
        pop();
        stack[ip] = stack[ip] / top;
    }
    else error("division by zero");
}