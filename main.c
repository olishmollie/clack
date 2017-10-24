#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <editline/readline.h>

#define BUFSIZE 150

typedef enum tokenType { number, error, operation } tokenType;
typedef struct token {
    tokenType type;
    int val;
    char* err;
    char* op;
} token;

token *newToken(tokenType type)
{
    token *tk = malloc(sizeof(token));
    if (tk) {
	tk->type = type;
	tk->err = NULL;
	tk->op = NULL;
    }
    return tk;
}

void deleteToken(token *tk)
{
    if (tk->err)
	free(tk->err);
    if (tk->op)
	free(tk->op);
    free(tk);
}

typedef struct tokenizer {
    char *input;
    int pos;
    int len;

} tokenizer;

tokenizer *newTokenizer(char* input)
{
    tokenizer *tz = malloc(sizeof(tokenizer));
    if (tz) {
	tz->input = input;
	tz->len = strlen(input);
	tz->pos = 0;
    }
    return tz;
}

void deleteTokenizer(tokenizer *tz)
{
    free(tz);
}

token *parseDigit(tokenizer *tz)
{
    token *tk;
    if (!isdigit(tz->input[tz->pos])) {
	tk = newToken(error);
	tk->err = malloc(BUFSIZE*sizeof(char));
	snprintf(tk->err, BUFSIZE,
	    "Expected digit at pos %d, got '%c'", tz->pos, tz->input[tz->pos]);
	return tk;
    }

    return NULL;
}

void printToken(token *tk)
{
    switch(tk->type) {
	case error:
	    printf("Error: %s\n", tk->err);
	    break;
	case number:
	    printf("%d\n", tk->val);
	    break;
	default:
	    printf("Unknown token type\n");
    }
}

int main(void)
{
    while (1) {
	char *input = readline(">> ");
	add_history(input);

	tokenizer *tz = newTokenizer(input);
	token *tk = parseDigit(tz);
	printToken(tk);

	deleteToken(tk);
	free(input);
    }

    return 0;
}
