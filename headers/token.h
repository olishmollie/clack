#ifndef _TOKEN_H
#define _TOKEN_H

// Token types
#define NUM  256
#define DIV  257
#define MOD  258
#define ID   259
#define DONE 260

struct Token {
    int type;
    int value;
    Token() : type(0), value(0) {}
    Token(int t, int v) : type(t), value(v) {}
};

#endif