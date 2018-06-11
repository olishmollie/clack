#include <assert.h>

#include "../src/headers/global.h"

void test_add()
{
    char input[] = "40 + 2";
    parse(input);
    assert(stackpeek() == 42);
}

void test_sub()
{
    char input[] = "40 - 2";
    parse(input);
    assert(stackpeek() == 38);
}

void test_mul()
{
    char input[] = "42 * 3";
    parse(input);
    assert(stackpeek() == 126);
}

void test_div()
{
    char input[] = "42 / 6";
    parse(input);
    assert(stackpeek() == 7);
}

void test_negatives()
{
    char input[] = "-42 + 35";
    parse(input);
    assert(stackpeek() == -7);
}

void test_unary_operators()
{
    char input[] = "5 - --2";
    parse(input);
    assert(stackpeek() == 3);
}

void test_whitespace()
{
    char one[] = "5 *     2+3";
    parse(one);
    assert(stackpeek() == 13);

    char two[] = "5+34*2/2";
    parse(two);
    assert(stackpeek() == 39);
}

void test_order()
{
    char input[] = "5 + 3 * 2 / 2";
    parse(input);
    assert(stackpeek() == 8);
}

void test_grouping()
{
    char input[] = "(5 * (5 + (100 / 20)) - (4 * 2))";
    parse(input);
    assert(stackpeek() == 42);
}

int main(void) {

    test_add();
    test_sub();
    test_mul();
    test_div();

    test_negatives();
    test_unary_operators();
    test_whitespace();
    test_order();
    test_grouping();

    return 0;
}
