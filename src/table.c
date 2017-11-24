#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/table.h"

struct table_t {
    int size;
    int count;
    char **keys;
    int *vals;
};

int hash(char *name)
{
    int h = 0;
    int len = strlen(name);
    for (int i = 0; i < len; i++) {
        h += name[i] << 2;
    }
    return h % MAXTABLESIZE;
}

table *table_new()
{
    table *t = malloc(sizeof(table));
    if (t) {
        t->size = MAXTABLESIZE;
        t->count = 0;
        t->vals = calloc((size_t)MAXTABLESIZE, sizeof(int));
        t->keys = calloc((size_t)MAXTABLESIZE, sizeof(char*));
    }
    return t;
}

int table_insert(table* t, char *k, int v)
{
    int h = hash(k);
    t->keys[h] = k;
    t->vals[h] = v;
    t->count++;
    return v;
}

int table_getvalue(table *t, char *k)
{
    int h = hash(k);
    if (t->vals[h]) {
        return t->vals[h];
    }
    return 0;
}

void table_print(table *t)
{
    int i, count;
    printf("{");
    for (i = 0, count = 0; i < t->size; i++) {
        if (t->vals[i]) {
            char *delim = (count == t->count - 1) ? "" : ", ";
            printf("%s: %d%s", t->keys[i], t->vals[i], delim);
            count++;
        }
    }
    printf("}\n");
}

void table_delete(table *t)
{
    if (t) {
        if (t->vals) {
            free(t->vals);
        }
    free(t);
    }
}
