#ifndef _CEV_OPERATORS_H
#define _CEV_OPERATORS_H

#define OPLIST_SIZE  (sizeof op_list / sizeof *op_list)

extern const char *op_list[];
extern const unsigned int op_preclist[];
extern int (*const op_funclist[])(cev_t *, token_t *);

int opindex(token_t *tk);
int op_pow(cev_t *cev, token_t *op);
int op_mult(cev_t *cev, token_t *op);

#endif /* _CEV_OPERATORS_H */
