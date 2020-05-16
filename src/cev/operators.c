#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "cev.h"
#include "operators.h"

/** Operators list */
const char *op_list[] = {
  "**", "^",  "*",  "/",
  "%",  "+",  "-",  "!",
  "&",  "|",  ">",  "<"
  ">=", "<=", "==", "&&",
  "||", "=",  "+=", "-=",
  "/="
};

/** Operators precedence */
const unsigned int op_preclist[] = {
  50, 50, 49, 49,
  49, 48, 48, 40,
  39, 39, 38, 38,
  38, 38, 38, 37,
  37, 30, 30, 30,
  30
};

int (*const op_funclist[])(cev_t *, token_t *) = {
  op_pow, NULL, op_mult,
};

/** Returns the operand index */
int opindex(token_t *tk)
{
  for (int i = 0; i < OPLIST_SIZE; i++) {
    if ( !strcmp(op_list[i], tk->text) )
      return i;
  }

  return -1;
}

int op_pow(cev_t *cev, token_t *op)
{
  token_t *v2 = stack_pop(&cev->stack);
  token_t *v1 = stack_pop(&cev->stack);

  if ( !v1 || !v2 ) {
    cev_error(op->line, op->start, op->end, "Unexpected binary operator here");
    return false;
  }

  if (v2->value < 0) {
    cev_error(v2->line, v2->start, v2->end, "Power operator expects a non-negative exponent.");
    return false;
  }

  if (v2->value == 0) {
    v1->value = 1;
  } else {
    v2->value--;
    for (int i = v1->value; v2->value; v2->value--)
      v1->value *= i;
  }

  stack_push(&cev->stack, v1);
  free(v2);
  return true;
}

int op_mult(cev_t *cev, token_t *op)
{
  token_t *v2 = stack_pop(&cev->stack);
  token_t *v1 = stack_pop(&cev->stack);

  if ( !v1 || !v2 ) {
    cev_error(op->line, op->start, op->end, "Unexpected binary operator here");
    return false;
  }

  v1->value = v1->value * v2->value;
  stack_push(&cev->stack, v1);

  free(v2);
  return true;
}
