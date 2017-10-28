#ifndef token_h
#define token_h

typedef enum toktype {
    NUMBER, PLUS, MINUS, MULT, DIV,
    LPAREN, RPAREN, END
} toktype;

typedef struct token_t token;

token *token_new(toktype, int);
toktype token_gettype(token*);
char* token_tostr(token*);

void token_delete(token*);

#endif
