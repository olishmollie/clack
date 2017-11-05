#ifndef ast_h
#define ast_h

#include "token.h"

typedef struct ast_t ast;
struct ast_t {
    token *root;
    ast *left;
    ast *right;
};


ast *ast_binop(token*, ast*, ast*);
ast *ast_unaryop(token*, ast*);
ast *ast_num(token*);
ast *ast_var(token*);
ast *ast_err(token*);
toktype ast_gettype(ast*);
void ast_print(ast*);

void ast_delete(ast*);

#endif
