#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "stack.h"
#include "parser.h"
#include "lexer.h"
#include "evaluator.h"
#include "error.h"

#define EOS '\0'
#define NONE -1

#define NUM  256
#define ID   257
#define DONE 260

int tokenval;
int linenum;
int lookahead;

#endif