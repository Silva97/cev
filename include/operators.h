#ifndef _CEV_OPERATORS_H
#define _CEV_OPERATORS_H

#define OPLIST_SIZE  (sizeof op_list / sizeof *op_list)

/** Operators list */
static const char *op_list[] = {
  "**", "^"   "*",  "/",
  "%",  "+",  "-",  "!",
  "&",  "|",  ">",  "<"
  ">=", "<=", "==", "&&",
  "||", "=",  "+=", "-=",
  "/="
};

/** Operators precedence */
static const unsigned int op_preclist[] = {
  50, 50, 49, 49,
  49, 48, 48, 40,
  39, 39, 38, 38,
  38, 38, 38, 37,
  37, 30, 30, 30,
  30
};

#endif /* _CEV_OPERATORS_H */
