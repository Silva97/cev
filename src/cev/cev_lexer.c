#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "cev.h"

static int isvarname(int c)
{
  return isalnum(c) || c == '_';
}

/**
 * @brief Tokenize a mathetical expression.
 * 
 * @param input      The expression to tokenize.
 * @return token_t*  The list of tokens.
 * @return NULL      No has valid tokens in the input.
 */
token_t *cev_lexer(char *input)
{
  int i;
  char *start;
  char *line = input;
  char text[TKMAX];
  token_type_t type;
  long long int value;

  token_t *new = NULL;
  token_t *first = NULL;
  token_t *last;

  do {
    while ( isspace(*input) )
      input++;
    
    if ( ! *input )
      break;
    
    if (*input == '#') {
      while (*input && *input != '\n')
        input++;
      
      continue;
    }

    value = 0;
    start = input;

    switch (*input) {
    case '(':
      type = TK_OPENPARENS;
      strcpy(text, "(");
      input++;
      break;
    case ')':
      type = TK_CLOSEPARENS;
      strcpy(text, ")");
      input++;
      break;
    case '^':
    case '~':
      type = TK_OPERATOR;
      text[0] = *input;
      text[1] = '\0';
      input++;
      break;
    case '*':
      type = TK_OPERATOR;
      if (input[1] == '*') {
        strcpy(text, "**");
        input += 2;
        break;
      }

      if (input[1] == '=') {
        strcpy(text, "*=");
        input += 2;
        break;
      }

      strcpy(text, "*");
      input++;
      break;
    case '&':
    case '|':
    case '<':
    case '>':
      if (input[1] == *input) {
        type = TK_OPERATOR;
        text[0] = *input;
        text[1] = *input;
        text[2] = '\0';
        input += 2;
        break;
      }

      text[1] = '\0';
    case '+':
    case '-':
    case '/':
    case '%':
    case '!':
    case '=':
      type = TK_OPERATOR;
      text[0] = *input;
      if (input[1] == '=') {
        text[1] = '=';
        text[2] = '\0';
        input++;
      } else if (text[1] != *input){
        text[1] = '\0';
      }

      input++;
      break;

    default:
      if ( isdigit(*input) ) {
        value = strtoll(input, &input, 0);

        if ( isvarname(*input) ) {
          while ( isvarname(input[1]) )
            input++;
          cev_error(line, start, input, "Invalid name starting at number");
          return NULL;
        }

        type = TK_NUMBER;
        break;
      }

      if ( !isvarname(*input) ) {
        cev_error(line, start, input, "Invalid token");
        return NULL;
      }

      for (i = 0; isvarname(*input); i++) {
        if (i >= TKMAX) {
          cev_error(line, start, input, "Name's maximum size exceeded");
          return NULL;
        }

        text[i] = *input++;
      }

      text[i] = '\0';
      type = TK_ID;
    }

    last = new;
    new = calloc(1, sizeof *new);
    strcpy(new->text, text);
    new->type = type;
    new->value = value;
    new->last = last;
    new->line = line;
    new->start = start;
    new->end = input - 1;
    
    if (last)
      last->next = new;
    else
      first = new;
  } while (*input);

  return first;
}
