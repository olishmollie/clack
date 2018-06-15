#ifndef _STACK_H
#define _STACK_H

#define MAXSIZE 128

// Returns top of the stack.
int stackpeek();

// Overwrites top of the stack.
void stackset(int);

// Pushes on to the stack.
void stackpush(int);

// Pops off the stack.
int stackpop();

#endif
