#include "symtable.h"
#include "error.h"

#include <stdlib.h>
#include <string.h>

unsigned long
hash(char *name)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *name++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

int symtable_insert(char *name) {
    unsigned long h = hash(name);
    int idx = h % TABLESIZE;
    SymtableEntry s;
    strcpy(s.name, name);
    symtable[idx] = s;
    return idx;
}

int symtable_lookup(char *name)
{
    unsigned long h = hash(name);
    int idx = h % TABLESIZE;
    SymtableEntry s = symtable[idx];
    if (strcmp(s.name, name) == 0)
        return idx;
    return -1;
}

void symtable_print()
{
    for (int i = 0; i < TABLESIZE; i++) {
        printf("%s: ", symtable[i].name);
        printf("%d\n", symtable[i].ival);
    }
}