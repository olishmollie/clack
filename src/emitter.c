#include "../headers/global.h"

void emit(int t, int tval)
{
    switch (t) {
    case NUM: push(tval); break;
    case '+': iadd(); break;
    case '-': isub(); break;
    case '*': imul(); break;
    case '/': idiv(); break;
    default:
        printf("token %d, tokenval %d\n", t, tval);
    }
}