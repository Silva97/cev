#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <inttypes.h>
#include "cev.h"
#include "tree.h"

void show_cev(cev_t *ctx, int64_t number);
void show_help(void);


int main(int argc, char **argv)
{
  char line[256];
  int ch;
  char *script = NULL;
  cev_t *ctx = calloc(1, sizeof *ctx);

  while ( (ch = getopt(argc, argv, "hs:")) > 0 ) {
    switch (ch) {
    case 'h':
      show_help();
      return EXIT_SUCCESS;
    case 's':
      script = optarg;
      break;
    case '?':
      return EXIT_FAILURE;
    }
  }

  for (int i = optind; i < argc; i++)
    cev(ctx, argv[i]);

  if (script) {
    int64_t output = cev_script(ctx, script);
    show_cev(ctx, output);
    return EXIT_SUCCESS;
  }

  fputs("Type 'quit' to exit.\n(cev) ", stdout);
  while ( fgets(line, sizeof line - 1, stdin) ) {
    if (line[0] == '\n')
      continue;
    if ( !strcmp(line, "quit\n") )
      break;

    show_cev(ctx, cev(ctx, line));
  }

  return EXIT_SUCCESS;
}

void show_cev(cev_t *ctx, int64_t number)
{
  int64_t base = 10;
  var_t *out = tree_find(ctx->vartree, hash("out"));
  if (out)
    base = out->value;

  switch (base) {
  case 8:
    printf("0%" PRIo64 "\n", number);
    break;
  case 16:
    printf("0x%" PRIx64 "\n", number);
    break;
  default:
    printf("%" PRId64 "\n", number);
    break;
  }
}

void show_help(void)
{
  puts(
    "Developed by Luiz Felipe <felipe.silva337@yahoo.com>\n"
    "  GitHub: https://github.com/Silva97/cev\n\n"

    "cev is a expression evaluator developed in C language. You can"
    "evaluate mathetical expressions with C-like operators and define"
    "variables to use in the expressions."

    "Usage: cev [options] [expr1 expr2 ...]\n"
    "  -h         Show this help message\n"
    "  -s <file>  Read from the file the expressions to execute\n\n"

    "VARIABLES\n"
    "  Variables can be set by command-line argument and in the expressions.\n"
    "  To set in the command-line, just insert the variable and your value:\n"
    "    ./cev x=42\n"
    "  Any positional argument can be any valid expression.\n\n"

    "EXPRESSIONS\n"
    "  Expressions uses the same operators in C language, including attribution operators.\n"
    "  The variables is int64_t types with 64-bit size and signal. To set a variable, just\n"
    "  Use attribution operators of the C language, like in:\n"
    "    (cev) x = 42\n\n"

    "  Expressions can be read from a input file (like a script) using -f option, in this\n"
    "  case each line of the file is evaluated like an expression. The latest expression\n"
    "  result is printed in the output."
  );
}
