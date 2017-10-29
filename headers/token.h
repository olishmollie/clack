#ifndef token_h
#define token_h

typedef enum toktype {
    NUMBER, PLUS, MINUS, TIMES, DIVIDE,
    LPAREN, RPAREN, IDENT, ERR, END
} toktype;

typedef struct token_t token;

token *token_new(toktype, int, char*);
toktype token_gettype(token*);
char* tokname(toktype);
char* token_tostr(token*);

void token_delete(token*);

#endif
