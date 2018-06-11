#include "../headers/global.h"

void emit(int t, int tval)
{
    switch (t) {
    case '+': case '-': case '*': case '/':
        printf("%c\n", t); break;
    case NUM:
        printf("%d\n", tval); break;
    default:
        printf("token %d, tokenval %d\n", t, tval);
    }
}