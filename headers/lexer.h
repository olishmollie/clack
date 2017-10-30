#ifndef lexer_h
#define lexer_h

#define MAXBUFSIZE 150

/* TOKEN */
typedef enum toktype {
    NUMBER, PLUS, MINUS, TIMES, DIVIDE,
    LPAREN, RPAREN, IDENT, ERR, END
} toktype;
char* tokname(toktype);

typedef struct token_t token;

token *token_new(toktype, int, char*);
toktype token_gettype(token*);
int token_getvalue(token*);
char* token_str(token *);

void token_delete(token*);

/* LEXER */
typedef struct lexer_t lexer;

lexer *lexer_new(char*);

token *lexer_peek(lexer*);
token *lexer_next(lexer*);
int lexer_getline(lexer*);
int lexer_getcol(lexer*);
int lexer_eof(lexer*);
void lexer_halt(lexer*);

void lexer_delete(lexer*);

#endif
