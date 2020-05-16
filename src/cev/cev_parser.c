#include <string.h>
#include <stdbool.h>
#include "cev.h"
#include "operators.h"

/** Returns the operator precedence */
unsigned int op_prec(token_t *tk)
{
  int i = opindex(tk);
  if (i < 0)
    return 0;

  return op_preclist[i];
}

/**
 * @brief Parse a token list to an cev_t struct.
 * 
 * @param cev   The cev_t to write the parsed tokens.
 * @param tk    The token list.
 * @return int  Returns false if error.
 */
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
      while ( true ) {
        poped = stack_pop(&cev->stack);
        if ( !poped ) {
          cev_error(tk->line, tk->start, tk->end, "Unopened parens.");
          return false;
        }
  
        if (poped->type == TK_OPENPARENS)
          break;
        
        queue_insert(&cev->queue, poped);
      }

      continue;
    }

    if ( cev->stack && op_prec(cev->stack->tk) >= op_prec(tk) )
      queue_insert(&cev->queue, stack_pop(&cev->stack));
    
    stack_push(&cev->stack, tk);
  }

  while ( (tk = stack_pop(&cev->stack)) ) {
    if (tk->type == TK_OPENPARENS) {
      cev_error(tk->line, tk->start, tk->end, "Unclosed parens.");
      return false;
    }

    queue_insert(&cev->queue, tk);
  }

  return true;
}
