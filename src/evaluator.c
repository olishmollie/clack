#include "headers/global.h"

void eval(int t)
{
    switch (t) {
    case NUM: push(tokenval); break;
    case '+': iadd(); break;
    case '-': isub(); break;
    case '*': imul(); break;
    case '/': idiv(); break;
    default:
        ;
    }
}