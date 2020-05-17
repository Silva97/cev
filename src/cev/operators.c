#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "cev.h"
#include "operators.h"

/** Operators list */
const char *op_list[] = {
  "**", "*",  "/",  "%",
  "^",  "+",  "-",  "!",
  "&",  "|",  ">",  "<"
  ">=", "<=", "==", "&&",
  "||", "=",  "+=", "-=",
  "/="
};

/** Operators precedence */
const unsigned int op_preclist[] = {
  50, 49, 49, 49,
  48, 47, 47, 40,
  39, 39, 38, 38,
  38, 38, 38, 37,
  37, 30, 30, 30,
  30
};

int (*const op_funclist[])(cev_t *, token_t *) = {
  op_pow, op_mult, op_div, op_mod,
  op_xor, op_plus, op_minus, op_log_not,
  op_and, op_or, op_gt, op_lt,
  op_ge, op_le, op_equ, op_log_and
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

int op_pow(OPARGS)
{
  OPV2_START

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

  OPV2_END
}

int op_mult(OPARGS)
{
  OPV2_START
  v1->value = v1->value * v2->value;
  OPV2_END
}

int op_div(OPARGS)
{
  OPV2_START
  if ( !v2 ) {
    cev_error(v2->line, v2->start, v2->end, "Division by zero");
    return false;
  }

  v1->value = v1->value / v2->value;
  OPV2_END
}

int op_mod(OPARGS)
{
  OPV2_START
  if ( !v2 ) {
    cev_error(v2->line, v2->start, v2->end, "Modulous by zero");
    return false;
  }

  v1->value = v1->value % v2->value;
  OPV2_END
}

int op_xor(OPARGS)
{
  OPV2_START
  v1->value = v1->value ^ v2->value;
  OPV2_END
}

int op_plus(OPARGS)
{
  OPV2_START
  v1->value = v1->value + v2->value;
  OPV2_END
}

int op_minus(OPARGS)
{
  token_t *v2 = stack_pop(&cev->stack);
  token_t *v1 = stack_pop(&cev->stack);
  if ( !v2 ) {
    cev_error(op->line, op->start, op->end, "Operator minus expects a operand");
    return false;
  }

  if ( v1 && v1->type == TK_NUMBER ) {
    v1->value = v1->value - v2->value;
    OPV2_END
  }

  if (v1 && v1->type == TK_OPERATOR)
    stack_push(&cev->stack, v1);

  v2->value = -v2->value;
  stack_push(&cev->stack, v2);
  return true;
}

int op_log_not(OPARGS)
{
  OPV1_START
  vv->value = !vv->value;
  OPV1_END
}

int op_and(OPARGS)
{
  OPV2_START
  v1->value = v1->value & v2->value;
  OPV2_END
}

int op_or(OPARGS)
{
  OPV2_START
  v1->value = v1->value | v2->value;
  OPV2_END
}

int op_gt(OPARGS)
{
  OPV2_START
  v1->value = v1->value > v2->value;
  OPV2_END
}

int op_lt(OPARGS)
{
  OPV2_START
  v1->value = v1->value < v2->value;
  OPV2_END
}

int op_ge(OPARGS)
{
  OPV2_START
  v1->value = v1->value >= v2->value;
  OPV2_END
}

int op_le(OPARGS)
{
  OPV2_START
  v1->value = v1->value <= v2->value;
  OPV2_END
}

int op_equ(OPARGS)
{
  OPV2_START
  v1->value = v1->value == v2->value;
  OPV2_END
}

int op_log_and(OPARGS)
{
  OPV2_START
  v1->value = v1->value && v2->value;
  OPV2_END
}

int op_log_or(OPARGS)
{
  OPV2_START
  v1->value = v1->value || v2->value;
  OPV2_END
}
