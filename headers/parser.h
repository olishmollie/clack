#ifndef interpreter_h
#define interpreter_h

#include "ast.h"
#include "lexer.h"

ast *factor(lexer*);
ast *term(lexer*);
ast *expr(lexer*);

#endif
