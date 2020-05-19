#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#include "cev.h"

/**
 * @brief Evaluate an expression using a context
 * 
 * The context has the variables' values.
 * 
 * @param ctx       If NULL, don't use a context.
 * @param input     The expression to evaluate.
 * @return int64_t  The result of the expression.
 */
int64_t cev(cev_t *ctx, char *input)
{
  cev_t *cev;
  int64_t ret;
  if (ctx)
    cev = ctx;
  else
    cev = calloc(1, sizeof *cev);
  
  if ( !cev->vartree )
    cev->vartree = calloc(1, sizeof (var_t));


  token_t *tklist = cev_lexer(input);
  if ( !tklist )
    return 0;

  if ( !cev_parser(cev, tklist) )
    exit(EXIT_FAILURE);
  
  ret = cev_eval(cev);
  if ( !ctx )
    cev_free(cev);
  
  token_free(tklist);
  return ret;
}

/**
 * @brief Evaluate an script with expressions.
 * 
 * @param ctx       If NULL, don't use a context.
 * @param input     The file to read the expressions.
 * @return int64_t  The latest expression result.
 */
int64_t cev_script(cev_t *ctx, char *filename)
{
  char line[513];
  FILE *input;
  cev_t *cev_ctx;
  int64_t last = 0;

  if (ctx)
    cev_ctx = ctx;
  else
    cev_ctx = calloc(1, sizeof *cev_ctx);

  if ( !strcmp(filename, "-") )
    input = stdin;
  else
    input = fopen(filename, "r");
  
  if ( !input ) {
    fprintf(stderr, CC "Error: " CE "File '%s' not found\n", filename);
    exit(EXIT_FAILURE);
  }

  while ( fgets(line, sizeof line - 1, input) ) {
    if (line[0] == '\n')
      continue;
    if ( !strcmp(line, "quit\n") )
      break;

    last = cev(cev_ctx, line);
  }

  fclose(input);
  if ( !ctx )
    cev_free(cev_ctx);

  return last;
}
