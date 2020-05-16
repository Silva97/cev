#include <string.h>
#include <stdbool.h>
#include "cev.h"
#include "operators.h"

/** Returns the operator precedence */
unsigned int op_prec(token_t *tk)
{
  if (tk->type != TK_OPERATOR)
    return 9999;

  for (int i = 0; i < OPLIST_SIZE; i++) {
    if ( !strcmp(op_list[i], tk->text) )
      return op_preclist[i];
  }

  return 0;
}

int cev_parser(cev_t *cev, token_t *tk)
{
  token_t *poped;

  for (; tk; tk = tk->next) {
    if (tk->type == TK_NUMBER || tk->type == TK_ID) {
      queue_insert(&cev->queue, tk);
      continue;
    }

    if (tk->type == TK_OPENPARENS) {
      stack_push(&cev->stack, tk);
      continue;
    }

    if (tk->type == TK_CLOSEPARENS) {
      while ( (poped = stack_pop(&cev->stack)) ) {
        if (poped->type == TK_OPENPARENS)
          break;
        
        queue_insert(&cev->queue, poped);
      }

      if ( !poped ) {
        cev_error(tk->line, tk->start, tk->end, "Unexpected close parens.");
        return false;
      }

      continue;
    }

    if ( cev->stack && op_prec(cev->stack->tk) < op_prec(tk) )
      queue_insert(&cev->queue, stack_pop(&cev->stack));
    
    stack_push(&cev->stack, tk);
  }

  while ( (tk = stack_pop(&cev->stack)) )
    queue_insert(&cev->queue, tk);

  return true;
}
