#ifndef token_h
#define token_h

#define MAXBUFSIZE 500

typedef enum toktype {
    MAIN, INT, FLOAT, BOOL, STRING, PLUS, MINUS, TIMES, DIVIDE,
    ASSIGN, EQUALS, NEQUALS, LT, LTE, GT, GTE, LPAREN, RPAREN,
    LBRACE, RBRACE, QUOTE, COMMA, POUND, IDENT, ERR, END, NWLN, NOOP
} toktype;
char* tokname(toktype);

typedef struct token_t token;

token *token_new(toktype, char*, char*);
toktype token_gettype(token*);
char *token_getvalue(token*);
char *token_geterr(token*);
char* token_str(token *);

void token_delete(token*);

#endif

