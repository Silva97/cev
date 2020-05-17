#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "cev.h"
#include "metric.h"

test_t test_queue(void)
{
  token_t *tk;
  cev_queue_t *queue = NULL;

  for (int i = 0; i < 10; i++) {
    tk = calloc(1, sizeof *tk);
    tk->type = TK_NUMBER;
    tk->value = i;
    queue_insert(&queue, tk);
  }

  for (int i = 0; (tk = queue_pop(&queue)); i++) {
    printf("%" PRId64 ", ", tk->value);
    METRIC_ASSERT(tk->value == i);
  }
  
  putchar('\n');
  queue_free(queue);

  METRIC_TEST_OK("");
}

test_t test_stack(void)
{
  token_t *tk;
  cev_stack_t *stack = NULL;

  for (int i = 0; i < 10; i++) {
    tk = calloc(1, sizeof *tk);
    tk->type = TK_NUMBER;
    tk->value = i;
    stack_push(&stack, tk);
  }

  for (int i = 9; (tk = stack_pop(&stack)); i--) {
    printf("%" PRId64 ", ", tk->value);
    METRIC_ASSERT(tk->value == i);
  }
  
  putchar('\n');
  stack_free(stack);

  METRIC_TEST_OK("");
}

test_t test_parser(void)
{
  cev_t *cev = calloc(1, sizeof *cev);
  token_t *tk = cev_lexer("24 ** 4 + (4-95) / ((27-13) ^ 85)");
  const token_type_t list[] = {
    TK_NUMBER, TK_NUMBER, TK_OPERATOR, TK_NUMBER,
    TK_NUMBER, TK_OPERATOR, TK_NUMBER, TK_NUMBER,
    TK_OPERATOR, TK_NUMBER, TK_OPERATOR, TK_OPERATOR,
    TK_OPERATOR
  };

  METRIC_ASSERT( cev_parser(cev, tk) );

  for (int i = 0; (tk = queue_pop(&cev->queue)); i++) {
    if (tk->type == TK_NUMBER)
      printf("%" PRId64 " ", tk->value);
    else
      printf("'%s' ", tk->text);
    
    METRIC_ASSERT(tk->type == list[i]);
  }
  putchar('\n');

  METRIC_TEST_OK("");
}

int main(void)
{
  METRIC_TEST(test_queue);
  METRIC_TEST(test_stack);
  METRIC_TEST(test_parser);

  METRIC_TEST_END();
  return metric_count_tests_fail;
}
