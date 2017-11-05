#ifndef parser_h
#define parser_h

#include "ast.h"
#include "lexer.h"

ast *factor(lexer*);
ast *term(lexer*);
ast *expr(lexer*);
ast *parse(lexer*);

#endif
