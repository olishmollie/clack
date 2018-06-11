#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "parser.h"
#include "lexer.h"
#include "emitter.h"
#include "error.h"

#define EOS '\0'
#define NONE -1

#define NUM  256
#define ID   257
#define DONE 260

extern int tokenval;
extern int linenum; 
extern int lookahead;

#endif