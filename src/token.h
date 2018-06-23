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
   tokenDBLSTAR,
   tokenSLASH,
   tokenPERCENT,
   tokenAMPERSAND,
   tokenPIPE,
   tokenLCARAT,
   tokenLSHIFT,
   tokenLOG,
   tokenLN,
   tokenPOW,
   tokenIDENT,
   tokenSPACE
} TokenType;

Token NewToken(int type, char *val);

void TokenPrint(Token t);
void TokenDelete(Token t);

#endif
