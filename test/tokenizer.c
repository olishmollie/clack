#include "../src/tokenizer.h"

#include <string.h>
#include <assert.h>

void testToken(char *input, TokenType type, char *tokenType)
{
    printf("Testing token %s -- ", tokenType);
    Tokenizer *t = TokenizerInit(input);
    Token tok = lexan(t);
    assert(tok.type == type);
    assert(strcmp(tok.val, input) == 0);
    free(t);
    TokenDelete(tok);
    printf("Test passed.\n\n");
}

int main(void)
{
    testToken("42", tokenINT, "int");
    testToken("3.14159", tokenFLOAT, "float");
    testToken("foobar", tokenIDENT, "ident");
    testToken("log", tokenLOG, "log");
    testToken("ln", tokenLN, "ln");

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

    return 0;
}
