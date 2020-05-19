#include <stdio.h>
#include "cev.h"

/**
 * @brief Prints a error message
 * 
 * @param line     The line with the error
 * @param start    The start char of the problem
 * @param end      The end char
 * @param message  The error message
 */
void cev_error(char *line, char *start, char *end, char *message)
{
  int size = (int) (end - start);
  int init = (int) (start - line);

  fprintf(stderr, CC "Error: " CE "%s\n", message);

  for (; *line && *line != '\n'; line++) {
    if (line == start)
      fputs(CC, stderr);
    
    putc(*line, stderr);

    if (line == end)
      fputs(CE, stderr);
  }

  if (init > 0)
    fprintf(stderr, "\n%*c" CC "^", init, ' ');
  else
    fputs(CC "\n^", stderr);

  while (size-- > 0) {
    putc('~', stderr);
  }
  
  fputs(CE "\n", stderr);
}
