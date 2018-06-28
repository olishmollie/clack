#ifndef _SYMTABLE_H
#define _SYMTABLE_H

#include "token.h"

#define TABLESIZE 20

typedef struct SymtableEntry {
    TokenType type;
    char name[MAXLEN];
    union {
            int ival;
        double fval;
    };
} SymtableEntry;

SymtableEntry symtable[TABLESIZE];

int symtable_insert(char *name);
int symtable_lookup(char *name);
void symtable_print();

#endif