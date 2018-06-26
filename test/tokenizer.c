#include "../src/tokenizer.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

void testToken(char *input, TokenType type, char *tokenType)
{
    printf("Testing token %s -- ", tokenType);
    Tokenizer *t = tokenizer_init(input);
    Token tok = lexan(t);
    assert(tok.type == type);
    assert(strcmp(tok.val, input) == 0);
    tokenizer_delete(t);
    printf("Test passed.\n\n");
}

typedef struct TokenTest {
    TokenType type;
    char * val;
    char * typeName;
} TokenTest;

void testExpression(char *input, TokenTest key[])
{
    Tokenizer *t = tokenizer_init(input);
    Token tok;
    int i;
    for (i = 0, tok = lexan(t); tok.type != tokenEOF; i++, tok = lexan(t)) {
        printf("Testing %s -- ", key[i].typeName);
        assert(tok.type == key[i].type);
        assert(strcmp(tok.val, key[i].val) == 0);
        printf("Test passed.\n\n");
    }
    tokenizer_delete(t);
}

int main(void)
{
    testToken("42", tokenINT, "int");
    testToken("3.14159", tokenFLOAT, "float");
    testToken("3/4", tokenRAT, "rational");
    testToken("foobar", tokenIDENT, "ident");

    testToken("/", tokenSLASH, "slash");
    testToken("&", tokenAMPERSAND, "ampersand");
    testToken("|", tokenPIPE, "pipe");
    testToken("<", tokenLCARAT, "lcarat");
    testToken("<<", tokenLSHIFT, "rshift");

    testToken("+", tokenPLUS, "plus");
    testToken("-", tokenMINUS, "minus");
    testToken("*", tokenSTAR, "star");
    testToken("**", tokenDBLSTAR, "dblstar");
    testToken("/", tokenSLASH, "slash");

    char *expr = "(3 + ln(4))/(4**x - 3.14159)";
    TokenTest key[] = {
        { tokenLPAREN, "(", "lparen" },
        { tokenINT, "3", "int" },
        { tokenPLUS, "+", "plus" },
        { tokenIDENT, "ln", "ln" },
        { tokenLPAREN, "(", "lparen" },
        { tokenINT, "4", "int" },
        { tokenRPAREN, ")", "rparen" },
        { tokenRPAREN, ")", "rparen" },
        { tokenSLASH, "/", "slash" },
        { tokenLPAREN, "(", "lparen" },
        { tokenINT, "4", "int" },
        { tokenDBLSTAR, "**", "dblstar" },
        { tokenIDENT, "x", "ident" },
        { tokenMINUS, "-", "minus" },
        { tokenFLOAT, "3.14159", "float" },
        { tokenRPAREN, ")", "rparen" }
    };
    testExpression(expr, key);

    return 0;
}
