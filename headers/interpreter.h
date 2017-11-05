#ifndef interpreter_h
#define interpreter_h

#include "token.h"
#include "ast.h"
#include "table.h"

int visit(ast*, table*);

#endif
