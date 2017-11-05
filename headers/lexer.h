#ifndef lexer_h
#define lexer_h

#include "token.h"

typedef struct lexer_t lexer;

lexer *lexer_new(char*);

token *lexer_currtok(lexer*);
token *lexer_advance(lexer*);
int lexer_getline(lexer*);
int lexer_getcol(lexer*);
int lexer_eof(lexer*);
void lexer_halt(lexer*);

void lexer_delete(lexer*);

#endif
