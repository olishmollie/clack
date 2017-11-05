#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/table.h"

struct table_t {
    int size;
    int *arr;
};

int hash(char *name)
{
    int h = 0;
    for (int i = 0; i < strlen(name); i++) {
	h += name[i] << 2;
    }
    return h % MAXTABLESIZE;
}

table *table_new()
{
    table *t = malloc(sizeof(table));
    if (t) {
        t->size = MAXTABLESIZE;
        t->arr = calloc((size_t)MAXTABLESIZE, sizeof(int));
    }
    return t;
}

int table_insert(table* t, char *k, int v)
{
    int h = hash(k);
    t->arr[h] = v;
    return v;
}

int table_getvalue(table *t, char *k)
{
    int h = hash(k);
    if (t->arr[h]) {
	return t->arr[h];
    }
    return 0;
}

void table_delete(table *t)
{
    if (t) {
        if (t->arr) {
            free(t->arr);
        }
    free(t);
    }
}
