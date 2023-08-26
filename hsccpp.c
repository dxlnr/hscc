/*
 *  HSCC: C Compiler Preprocessor
 * 
*/

#include "hscc.h"

const char * const token_str[] = {
  #define TOK(id, str) str,
    #include "include/toks.h"
  #undef TOK
};

/* find a token and add it to table. */
t_tokens *tok_add(const char *str, int len)
{
}

/* finds a tokens str. */
const char *get_tok_str(int v)
{
}
