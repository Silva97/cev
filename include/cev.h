#ifndef _CEV_H
#define _CEV_H

#include <stdio.h>
#include <inttypes.h>
#include "tree.h"

#define TKMAX 33

#ifdef _WIN32
# define CC ""
# define CE ""
#else
# define CC "\x1b[91m"
# define CE "\x1b[0m"
#endif


typedef enum token_type {
  TK_ID,
  TK_NUMBER,
  TK_OPERATOR,
  TK_OPENPARENS,
  TK_CLOSEPARENS
} token_type_t;

typedef struct token {
  struct token *next;
  struct token *last;

  char *line;
  char *start;
  char *end;
  token_type_t type;
  char text[TKMAX];
  int64_t value;
} token_t;

/** Syntax queue */
typedef struct cev_queue {
  struct cev_queue *next;
  
  token_t *tk;
} cev_queue_t;

/** Syntax stack */
typedef struct cev_stack {
  struct cev_stack *last;
  
  token_t *tk;
} cev_stack_t;

/** Variable's tree */
typedef struct var {
  EXTENDS_TREE(var);

  int64_t value;
} var_t;

/** Context to an expression */
typedef struct cev {
  var_t *vartree;
  cev_stack_t *stack;
  cev_queue_t *queue;
} cev_t;


void cev_free(cev_t *cev);
void token_free(token_t *tk);
void queue_insert(cev_queue_t **queue, token_t *tk);
token_t *queue_pop(cev_queue_t **queue);
void queue_free(cev_queue_t *queue);
void stack_push(cev_stack_t **stack, token_t *tk);
token_t *stack_pop(cev_stack_t **stack);
void stack_free(cev_stack_t *stack);

void cev_error(char *line, char *start, char *end, char *message);
int64_t cev(cev_t *ctx, char *input);
int64_t cev_script(cev_t *ctx, char *filename);
token_t *cev_lexer(char *input);
unsigned int op_prec(token_t *tk);
int cev_parser(cev_t *cev, token_t *tk);
void op_eval(cev_t *cev, token_t *op);
int64_t cev_eval(cev_t *cev);

#endif /* _CEV_H */
