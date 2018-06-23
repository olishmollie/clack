#include "src/tokenizer.h"
#include "src/token.h"

#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>

int main(int argc, char** argv) {

  puts("Fig Version 0.02");
  puts("Press Ctrl+c to Exit\n");

  while (1) {
    char *input = readline("fig> ");
    add_history(input);
    Tokenizer *t = TokenizerInit(input);
    TokenizerRun(t);
    TokenizerDelete(t);
  }

  return 0;
}
