/*
 *  HSCC C Compiler
 * 
*/

#ifndef HSCC_H
#define HSCC_H

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Tokens */
typedef struct t_tokens {
} t_tokens;


#define T_IDENT 0x1

typedef enum token_type {
  t_eof = T_IDENT - 1
#define TOK(id, str) ,id
  #include "include/toks.h"
#undef TOK
  ,t_ident
  ,t_string_literal
  ,t_numeric_const
} token_type_t;

extern const char * const token_str[];

/* Compiler state */
typedef struct cc_state {
  unsigned char verbose;
  unsigned char dump_ast;
  unsigned char dump_tokens;
} cc_state;


/* hsccpp */
const char* get_token_str(int id);

#endif
