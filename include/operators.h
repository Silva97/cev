#ifndef _CEV_OPERATORS_H
#define _CEV_OPERATORS_H

#define OPLIST_SIZE  (sizeof op_list / sizeof *op_list)
#define OPARGS cev_t *cev, token_t *op

#define OPV2_START \
  token_t *v2 = stack_pop(&cev->stack); \
  token_t *v1 = stack_pop(&cev->stack); \
  if ( !v1 || !v2 || v1->type != TK_NUMBER ) { \
    cev_error(op->line, op->start, op->end, "Unexpected binary operator here"); \
    return false; \
  }

#define OPV2_END \
  stack_push(&cev->stack, v1); \
  free(v2); \
  return true;

#define OPV1_START \
  token_t *vv = stack_pop(&cev->stack); \
  if ( !vv ) { \
    cev_error(op->line, op->start, op->end, "Unary operator expects a operand"); \
    return false; \
  }

#define OPV1_END \
  stack_push(&cev->stack, vv); \
  return true;

extern const char *op_list[];
extern const unsigned int op_preclist[];
extern int (*const op_funclist[])(cev_t *, token_t *);

int opindex(token_t *tk);
int op_pow(OPARGS);
int op_mult(OPARGS);
int op_div(OPARGS);
int op_mod(OPARGS);
int op_xor(OPARGS);
int op_plus(OPARGS);
int op_minus(OPARGS);
int op_log_not(OPARGS);
int op_and(OPARGS);
int op_or(OPARGS);
int op_gt(OPARGS);
int op_lt(OPARGS);
int op_ge(OPARGS);
int op_le(OPARGS);
int op_equ(OPARGS);
int op_log_and(OPARGS);
int op_log_or(OPARGS);

#endif /* _CEV_OPERATORS_H */
