#ifndef _TOKEN_H
#define _TOKEN_H

#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 20

typedef struct Token {
    int type;
    char val[MAXLEN];
} Token;

typedef enum TokenType {
   tokenEOF,
   tokenERROR,
   tokenEQUAL,
   tokenINT,
   tokenFLOAT,
   tokenRAT,
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
   tokenUPCARAT,
   tokenLCARAT,
   tokenLSHIFT,
   tokenIDENT,
   tokenBUILTIN,
   tokenSPACE
} TokenType;

Token token_new(int type, char *val, int len);
void token_print(Token t);

#endif
