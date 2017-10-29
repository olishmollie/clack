#ifndef interpreter_h
#define interpreter_h

#include "lexer.h"

token *intepreter_factor(lexer*);
token *interpreter_term(lexer*);
token *interpreter_expr(lexer*);

#endif
