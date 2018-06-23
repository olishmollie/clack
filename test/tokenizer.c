#include "../src/tokenizer.h"
#include "../src/token.h"

#include <string.h>
#include <assert.h>

void testToken(char *input, TokenType type)
{
    Tokenizer *t = TokenizerInit(input);
    Token tok = lexan(t);
    assert(tok.type == type);
    assert(strcmp(tok.val, input) == 0);
    free(t);
}

int main(void)
{
    testToken("42", tokenINT);
    testToken("3.14159", tokenFLOAT);
    testToken("log", tokenLOG);
    testToken("ln", tokenLN);
    testToken("pow", tokenPOW);
    return 0;
}
