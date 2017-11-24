#ifndef interpreter_h
#define interpreter_h

#include "token.h"
#include "ast.h"
#include "table.h"

void visit_prog(ast*, table*);
int visit_binop(ast*, table*);
void visit_fun(ast*, table*);

#endif
