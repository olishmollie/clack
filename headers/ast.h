#ifndef ast_h
#define ast_h

#include "token.h"

#define MAXPARAMS 50

typedef struct ast_stmt_t {
    token *root;
    struct ast_stmt_t *left;
    struct ast_stmt_t *right;
} ast_stmt;

typedef struct ast_stmtlist_t {
    ast_stmt **children;
    int num_children;
} ast_stmtlist;

ast_stmtlist *ast_stmtlist_new();
void ast_stmtlist_addchild(ast_stmtlist*, ast_stmt*);
ast_stmt *ast_binop(token*, ast_stmt*, ast_stmt*);
ast_stmt *ast_unaryop(token*, ast_stmt*);
ast_stmt *ast_num(token*);
ast_stmt *ast_var(token*);
ast_stmt *ast_noop(void);
toktype ast_gettype(ast_stmt*);
void ast_stmtlist_print(ast_stmtlist*);
void ast_stmt_print(ast_stmt*);
void ast_stmtlist_delete(ast_stmtlist*);
void ast_stmt_delete(ast_stmt*);

#endif
