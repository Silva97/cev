#include <stdio.h>
#include <inttypes.h>
#include "metric.h"
#include "cev.h"

test_t test_lexer(void)
{
  token_type_t list[] = {
    TK_NUMBER, TK_OPERATOR, TK_OPENPARENS, TK_NUMBER,
    TK_OPERATOR, TK_NUMBER, TK_CLOSEPARENS, TK_OPERATOR,
    TK_ID, TK_OPERATOR, TK_ID
  };

  token_t *tk = cev_lexer(" 23+(0x1a += 42)%=bbb   == ccc  ");
  if ( !tk )
    METRIC_TEST_FAIL("Lexer error");

  for (int i = 0; tk; i++) {
    if (tk->type == TK_NUMBER)
      printf("%" PRId64 " ", tk->value);
    else
      printf("'%s' ", tk->text);
    
    METRIC_ASSERT(tk->type == list[i]);
    tk = tk->next;
  }
  putchar('\n');

  METRIC_TEST_OK("");
}

int main(void)
{
  METRIC_TEST(test_lexer);

  METRIC_TEST_END();
  return metric_count_tests_fail;
}
