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
#include <stdint.h>

/* parse_args return codes: */
#define ARG_HELP 1


typedef struct file_buffer {
  uint8_t *buf_ptr;
  uint8_t *buf_end;
  char fn[512];
  /* file descriptor. 
   * Provides a handle for low-level operations and interactions with the operating system. 
   * It offers fine-grained control, efficiency, and versatility, 
   * especially when dealing with various types of I/O resources.
   */
  int fd;
} file_buffer_t;

struct file_spec {
    char type;
    char name[1];
};

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
  /* command line options */
  unsigned char verbose;
  unsigned char dump_ast;
  unsigned char dump_tokens;

  /* output file for preprocessing */
  FILE *ppfp;
  /* files from cli */
  struct file_spec **files; 
  /* output filename */
  char *outfile; 
  int fc;
} cc_state_t;


/* hsccpp */
const char* get_token_str(int id);

#endif
