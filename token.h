#ifndef token_h
#define token_h

typedef enum toktype toktype;
typedef struct token_t token;

token *token_new(toktype);
void token_setvalue(token*, void*);

#endif
