#ifndef _CEV_H
#define _CEV_H

#include <stdio.h>
#include <inttypes.h>
#include "tree.h"

#define TKMAX 33


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

/** Syntax tree */
typedef struct ast {
  struct ast *left;
  struct ast *right;
  
  token_t *tk;
} ast_t;

/** Variable's tree */
typedef struct var {
  EXTENDS_TREE(var);

  int64_t value;
} var_t;

/** Context to a expression */
typedef struct cev {
  var_t *vartree;
} cev_t;


void cev_error(char *line, char *start, char *end, char *message);
int64_t cev(cev_t *ctx, char *input);
token_t *cev_lexer(char *input);

#endif /* _CEV_H */
