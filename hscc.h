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

#include <fcntl.h>

/* parse_args return codes: */
#define ARG_HELP        1

#define CC_OUTPUT_OBJ   1 /* object file */

/* filetypes: */
#define FILE_TYPE_NONE   0
#define FILE_TYPE_C      1
#define FILE_TYPE_ASM    2

typedef struct file_buffer {
  /* buf_ptr equals buf_end : the buffer is considered empty. */
  uint8_t *buf_ptr;
  uint8_t *buf_end;
  /* file name */
  char fn[512];
  /* Linked list of file buffers */
  struct file_buffer *prev;
  /* line number for error reporting */
  int line_num;
  /* file descriptor. 
   *
   * Provides a handle for low-level operations and interactions with the operating system. 
   * It offers fine-grained control, efficiency, and versatility, 
   * especially when dealing with various types of I/O resources.
   */
  int fd;
  /* Dynarray */
  unsigned char buf[1];
} file_buffer_t;

typedef struct file_spec {
  char type;
  /* the intention is behind n[1] is to allocate a variable-length array at the end of the struct.
   * manually allocating more memory than the size of the struct to accommodate a longer array.
   * 
   * This is a common technique in C, but it is not part of the C standard. 
   */
  char name[1];
} file_spec_t;

/* Tokens Table */
typedef struct t_tokens {
} t_tokens_t;


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

  unsigned char filetype; 

  /* input file buffer */
  file_buffer_t *fb;
  /* output file for preprocessing */
  FILE *ppfp;
  /* files from cli */
  struct file_spec **files; 
  /* output filename */
  char *outfile; 
  int output_type;
  /* number of files */
  int fc;
} cc_state_t;

/* hsccpp */
const char* get_token_str(int id);

#endif
