#ifndef interpreter_h
#define interpreter_h

#include "token.h"
#include "ast.h"
#include "table.h"

void visit_statement_list(statement_list*, table*);
int visit(statement*, table*);

#endif
