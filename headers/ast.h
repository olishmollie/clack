#ifndef ast_h
#define ast_h

#include "token.h"

#define MAXSTATEMENTS 500

typedef struct astlist_t astlist;

typedef struct ast_t {
    token *root;
    struct ast_t *left;
    struct ast_t *right;
    astlist *args;
    astlist *body;
    astlist *elsebody;
} ast;

struct astlist_t {
    ast *children[MAXSTATEMENTS];
    int nchildren;
};

/* AST lists */
astlist *astlist_new();
void astlist_addchild(astlist*, ast*);
void astlist_append(astlist*, astlist*);

/* ASTs */
ast *ast_new();
ast *ast_binop(token*, ast*, ast*);
ast *ast_unaryop(token*, ast*);
ast *ast_num(token*);
ast *ast_var(token*);
ast *ast_str(token*);
ast *ast_loop(token*, ast*, astlist*);
ast *ast_branch(token*, ast*, astlist*, astlist*);
ast *ast_funcall(token*, astlist*);
ast *ast_noop(void);
ast *ast_fundef(token*, astlist*, astlist*);

/* Printing */
void ast_branch_print(ast*, int);
void ast_loop_print(ast*, int);
void ast_funcall_print(ast*, int);
void ast_fundef_print(ast*, int);
void astlist_print(astlist*, int);
void ast_print(ast*);
void astlist_delete(astlist*);
void ast_delete(ast*);

#endif
