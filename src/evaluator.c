#include "global.h"

void eval(int t)
{
    switch (t) {
    case NUM: stackpush(tokenval); break;
    case '+': iadd(); break;
    case '-': isub(); break;
    case '*': imul(); break;
    case '/': idiv(); break;
    default:
        ;
    }
}

void iadd() {
    int tmp = stackpop();
    stackset(stackpeek() + tmp);
}

void isub() {
    int tmp = stackpop();
    stackset(stackpeek() - tmp);
}

void imul() {
    int tmp = stackpop();
    stackset(stackpeek() * tmp);
}

void idiv() {
    int tmp = stackpop();
    if (tmp != 0) {
        stackset(stackpeek() / tmp);
    }
    else error("division by zero");
}
