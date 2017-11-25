#ifndef ast_h
#define ast_h

#include "token.h"

#define MAXPARAMS 50

typedef struct ast_ast_t ast;
struct ast_ast_t {
    token *root;
    ast *left;
    ast *right;
    ast **children;
    int num_children;
};

ast *ast_prog();
void ast_addparam(ast*, ast*);
void ast_addchild(ast*, ast*);
ast *ast_funcall(token*);
ast *ast_binop(token*, ast*, ast*);
ast *ast_unaryop(token*, ast*);
ast *ast_num(token*);
ast *ast_var(token*);
ast *ast_err(token*);
ast *ast_noop(void);
toktype ast_gettype(ast*);
void ast_print(ast*);
void ast_delete(ast*);

#endif
