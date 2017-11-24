#ifndef table_h
#define table_h

#define MAXTABLESIZE 127

typedef struct table_t table;

int hash(char*);
table *table_new();
int table_insert(table*, char*, int);
int table_getvalue(table*, char*);
void table_print(table*);

void table_delete(table*);

#endif
