#include <stdio.h>

#ifdef _WIN32
# define CC ""
# define CE ""
#else
# define CC "\x1b[91m"
# define CE "\x1b[0m"
#endif

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

  for (; *line; line++) {
    if (line == start)
      fputs(CC, stderr);
    
    putc(*line, stderr);

    if (line == end)
      fputs(CE, stderr);
  }

  fprintf(stderr, "\n%*c" CC "^", init, ' ');
  while (size-- > 0) {
    putc('~', stderr);
  }
  
  fputs(CE "\n", stderr);
}
