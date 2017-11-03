#ifndef ast_h
#define ast_h

#include "token.h"

typedef struct ast_t ast;

ast *ast_binop(token*, ast*, ast*);
ast *ast_num(token*);
void ast_print(ast*, int, char*);

void ast_delete(ast*);

#endif
