#ifndef parser_h
#define parser_h

#include "ast.h"
#include "lexer.h"

statement *factor(lexer*);
statement *term(lexer*);
statement *expr(lexer*);
statement_list *parse(lexer*);

#endif
