#include "src/tokenizer.h"
#include "src/parser.h"
#include "src/token.h"

#include <string.h>
#include <editline/readline.h>

int main(int argc, char **argv)
{

  puts("clack version 0.0.6");
  puts("Type 'exit' to quit\n");

  char *input;
  while (1)
  {
    char *input = readline("clack> ");
    if (strcmp(input, "exit") == 0)
    {
      free(input);
      break;
    }
    add_history(input);
    parse(input);
    free(input);
  }

  return 0;
}
