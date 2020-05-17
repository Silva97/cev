#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#include "cev.h"

/**
 * @brief Evaluate a expression using a context
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
