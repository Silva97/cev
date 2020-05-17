#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "cev.h"
#include "metric.h"

test_t test_eval(void)
{
  cev_t *cev = calloc(1, sizeof *cev);
  token_t *tk = cev_lexer("24 ** 4 + (4-95) / ((27-13) ^ 85)");
  
  METRIC_ASSERT( cev_parser(cev, tk) )
  METRIC_ASSERT( cev_eval(cev) == 331775 );

  METRIC_TEST_OK("");
}

int main(void)
{
  METRIC_TEST(test_eval);

  METRIC_TEST_END();
  return metric_count_tests_fail;
}
