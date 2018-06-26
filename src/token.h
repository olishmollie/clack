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

Token new_token(int type, char *val);

void token_print(Token t);
void token_delete(Token t);

#endif
