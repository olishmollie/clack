#include "src/global.h"

#include <editline/readline.h>

int main(int argc, char** argv) {

  puts("Fig Version 0.01");
  puts("Press Ctrl+c to Exit\n");

  while (1) {
    char *input = readline("fig> ");
    add_history(input);
    parse(input);
    printf("%d\n", stackpeek());
    free(input);
  }

  return 0;
}
