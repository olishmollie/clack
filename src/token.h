#ifndef _TOKEN_H
#define _TOKEN_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Token {
    int type;
    char *val;
} Token;

typedef enum TokenType {
   tokenEOF,
   tokenERROR,
   tokenINT,
   tokenFLOAT,
   tokenLPAREN,
   tokenRPAREN,
   tokenPLUS,
   tokenMINUS,
   tokenSTAR,
   tokenSLASH,
   tokenPERCENT,
   tokenLOG,
   tokenLN,
   tokenPOW,
   tokenIDENT
} TokenType;

Token NewToken(int type, char *val);

void TokenPrint(Token t);

#endif
