#ifndef token_h
#define token_h

#define MAXBUFSIZE 500

typedef enum toktype {
    NUMBER, PLUS, MINUS, TIMES, DIVIDE,
    LPAREN, RPAREN, IDENT, ERR, END, NIL
} toktype;
char* tokname(toktype);

typedef struct token_t token;

token *token_new(toktype, int, char*, char*);
toktype token_gettype(token*);
int token_getvalue(token*);
char* token_str(token *);

void token_delete(token*);

#endif

