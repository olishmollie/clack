#ifndef interpreter_h
#define interpreter_h

#include "lexer.h"

token *factor(lexer*);
token *term(lexer*);
token *expr(lexer*);

#endif
