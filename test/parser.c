#include "../src/parser.h"
#include "../src/evaluator.h"

#include <stdio.h>
#include <assert.h>

typedef struct MockExpr {
    char *input;
    StackEntry output;
} MockExpr;

MockExpr new_mockexpr(char *input, TokenType type, void *output)
{
    MockExpr m;
    m.input = input;
    StackEntry e;
    e.type = type;
    switch (type) {
    case tokenINT:
        e.ival = *(int*)output;
        break;
    case tokenFLOAT:
        e.fval = *(double*)output;
        break;
    default:
        fprintf(stderr, "bad type for mock expr\n");
    }
    m.output = e;
    return m;
}

void testExpression(MockExpr e)
{
    printf("Testing %s -- ", e.input);
    parse(e.input);
    switch (e.output.type) {
    case tokenINT:
        assert(e.output.ival == stack[top].ival);
        break;
    case tokenFLOAT:
        assert(e.output.fval == stack[top].fval);
        break;
    default:
        fprintf(stderr, "bad expression type\n");
        assert(1 == 0);
    }
    printf("Test passed.\n");
}

int main(void)
{
    char *arith = "(3 + 3 - (2 * -5)) / 8";
    int ans = 2;
    MockExpr m = new_mockexpr(arith, tokenINT, &ans);
    testExpression(m);
}