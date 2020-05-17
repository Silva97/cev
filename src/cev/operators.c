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
  op_ge, op_le, op_equ, op_log_and,
  op_log_or,
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

/** Gets the value of a number or variable token */
int64_t getv(cev_t *cev, token_t *tk)
{
  if ( !tk )
    return 0;

  if (tk->type == TK_NUMBER)
    return tk->value;
  
  if (tk->type != TK_ID) {
    cev_error(tk->line, tk->start, tk->end, "Unexpected token here");
    exit(EXIT_FAILURE);
  }

  var_t *var = tree_find(cev, hash(tk->text));
  if ( !var ) {
    cev_error(tk->line, tk->start, tk->end, "Undefined variable");
    exit(EXIT_FAILURE);
  }

  return var->value;
}

int op_pow(OPARGS)
{
  OPV2_START
  int64_t value2 = getv(cev, v2) - 1;

  if (value2 < 0) {
    cev_error(v2->line, v2->start, v2->end, "Power operator expects a non-negative exponent.");
    return false;
  }

  if (v2->value == 0) {
    lvalue = 1;
  } else {
    lvalue = getv(cev, v1);
    for (int i = lvalue; value2; value2--)
      lvalue *= i;
  }

  OPV2_END
}

int op_mult(OPARGS)
{
  OPV2_START
  lvalue = getv(cev, v1) * getv(cev, v2);
  OPV2_END
}

int op_div(OPARGS)
{
  OPV2_START
  int64_t value2 = getv(cev, v2);
  if ( !value2 ) {
    cev_error(v2->line, v2->start, v2->end, "Division by zero");
    return false;
  }

  lvalue = getv(cev, v1) / value2;
  OPV2_END
}

int op_mod(OPARGS)
{
  OPV2_START
  int64_t value2 = getv(cev, v2);
  if ( !value2 ) {
    cev_error(v2->line, v2->start, v2->end, "Modulous by zero");
    return false;
  }

  lvalue = getv(cev, v1) % value2;
  OPV2_END
}

int op_xor(OPARGS)
{
  OPV2_START
  lvalue = getv(cev, v1) ^ getv(cev, v2);
  OPV2_END
}

int op_plus(OPARGS)
{
  OPV2_START
  lvalue = getv(cev, v1) + getv(cev, v2);
  OPV2_END
}

int op_minus(OPARGS)
{
  OPV2_START
  lvalue = getv(cev, v1) - getv(cev, v2);
  OPV2_END
}

int op_log_not(OPARGS)
{
  OPV1_START
  lvalue = !getv(cev, vv);
  OPV1_END
}

int op_and(OPARGS)
{
  OPV2_START
  lvalue = getv(cev, v1) & getv(cev, v2);
  OPV2_END
}

int op_or(OPARGS)
{
  OPV2_START
  lvalue = getv(cev, v1) | getv(cev, v2);
  OPV2_END
}

int op_gt(OPARGS)
{
  OPV2_START
  lvalue = getv(cev, v1) > getv(cev, v2);
  OPV2_END
}

int op_lt(OPARGS)
{
  OPV2_START
  lvalue = getv(cev, v1) < getv(cev, v2);
  OPV2_END
}

int op_ge(OPARGS)
{
  OPV2_START
  lvalue = getv(cev, v1) >= getv(cev, v2);
  OPV2_END
}

int op_le(OPARGS)
{
  OPV2_START
  lvalue = getv(cev, v1) <= getv(cev, v2);
  OPV2_END
}

int op_equ(OPARGS)
{
  OPV2_START
  lvalue = getv(cev, v1) == getv(cev, v2);
  OPV2_END
}

int op_log_and(OPARGS)
{
  OPV2_START
  lvalue = getv(cev, v1) && getv(cev, v2);
  OPV2_END
}

int op_log_or(OPARGS)
{
  OPV2_START
  lvalue = getv(cev, v1) || getv(cev, v2);
  OPV2_END
}
