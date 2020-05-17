#include <stdlib.h>
#include "cev.h"

/** Free an cev_t struct */
void cev_free(cev_t *cev)
{
  tree_free(cev->vartree);
  stack_free(cev->stack);
  queue_free(cev->queue);
}

/** Free an token list */
void token_free(token_t *tk)
{
  while (tk) {
    if (tk->last)
      free(tk->last);
    
    if ( !tk->next ) {
      free(tk);
      break;
    }

    tk = tk->next;
  }
}

/** Insert in an queue */
void queue_insert(cev_queue_t **queue, token_t *tk)
{
  cev_queue_t *end;
  cev_queue_t *new = calloc(1, sizeof *new);
  new->tk = tk;
  
  if ( ! *queue ) {
    *queue = new;
    return;
  }

  for (end = *queue; end->next; end = end->next);

  end->next = new;
}

/** Popes from the queue */
token_t *queue_pop(cev_queue_t **queue)
{
  if ( ! *queue )
    return NULL;

  token_t *tk = (*queue)->tk;
  cev_queue_t *tofree = *queue;

  *queue = (*queue)->next;
  free(tofree);
  return tk;
}

void queue_free(cev_queue_t *queue)
{
  if ( !queue )
    return;

  cev_queue_t *last = queue;

  do {
    queue = queue->next;
    free(last->tk);
    free(last);
    last = queue;
  } while (queue);
}

/** Push to an stack */
void stack_push(cev_stack_t **stack, token_t *tk)
{
  cev_stack_t *new = calloc(1, sizeof *new);
  new->tk = tk;
  new->last = *stack;

  *stack = new;
}

/** Popes from the Stack */
token_t *stack_pop(cev_stack_t **stack)
{
  if ( ! *stack )
    return NULL;

  token_t *tk = (*stack)->tk;
  cev_stack_t *tofree = *stack;

  *stack = (*stack)->last;
  free(tofree);
  return tk;
}

void stack_free(cev_stack_t *stack)
{
  if ( !stack )
    return;

  cev_stack_t *last = stack;

  do {
    stack = stack->last;
    free(last->tk);
    free(last);
    last = stack;
  } while (stack);
}
