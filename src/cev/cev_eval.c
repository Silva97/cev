#include <stdlib.h>
#include <inttypes.h>
#include "cev.h"
#include "operators.h"

/** Evaluate an operator */
void op_eval(cev_t *cev, token_t *op)
{
  int i = opindex(op);
  if (i < 0) {
    cev_error(op->line, op->start, op->end, "Unexpected token");
    exit(EXIT_FAILURE);
  }

  if ( !op_funclist[i](cev, op) )
    exit(EXIT_FAILURE);
}

/** Evaluate an expression */
int64_t cev_eval(cev_t *cev)
{
  token_t *tk;
  if ( !cev->queue ) {
    cev_error("", "", "", "Empty queue, first use cev_parser()");
    exit(EXIT_FAILURE);
  }

  while ( (tk = queue_pop(&cev->queue)) ) {
    if (tk->type == TK_NUMBER || tk->type == TK_ID) {
      stack_push(&cev->stack, tk);
      continue;
    }

    op_eval(cev, tk);
  }

  return getv( cev, stack_pop(&cev->stack) );
}
