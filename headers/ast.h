#ifndef statement_h
#define statement_h

#include "token.h"

typedef struct ast_statement_t statement;
struct ast_statement_t {
    token *root;
    statement *left;
    statement *right;
};

typedef struct ast_list_t statement_list;
struct ast_list_t {
    statement **statements;
    int num_statements;
};


statement *statement_binop(token*, statement*, statement*);
statement *statement_unaryop(token*, statement*);
statement *statement_num(token*);
statement *statement_var(token*);
statement *statement_err(token*);
toktype statement_gettype(statement*);
void statement_print(statement*);
void statement_delete(statement*);

statement_list *statement_list_new();
void statement_list_add_statement(statement_list*, statement*);
void statement_list_delete(statement_list*);

#endif
