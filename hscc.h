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
#include <ctype.h>

#include <fcntl.h>
#include <setjmp.h>
#include <sys/stat.h>
#include <unistd.h>

/* parse_args return codes: */
#define ARG_HELP          1

#define CC_OUTPUT_OBJ     1 /* object file */

/* filetypes: */
#define FILE_TYPE_NONE    0
#define FILE_TYPE_C       1
#define FILE_TYPE_ASM     2

#define IO_BUF_SIZE       8192

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
  /* inline counter for error reporting */
  int inline_c;
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

/* Parser: Abstract Syntax Tree (AST)
 *
 * AST Types
 * */
typedef enum {
  AST_OPS,
  AST_LITERAL,
  AST_VAR_REF,
  AST_ASSIGN,
  AST_IF_ELSE,
  AST_FUNC,     
} ast_type_t;

#define MAX_CHILDREN     8

/* Single node in AST */
struct ast_node{
  ast_type_t ast_type;
  struct ast_node* child[MAX_CHILDREN];
  struct ast_node* next;
  /* for type checking of exps */
  /* exp_type_t exp_type; */ 
  struct{
    int op;
    int value;
    char* name;
  } attr;
  /* symbol_table_t* symbol_table; */
};


#define T_IDENT 256

typedef enum token_type {
  t_eof = T_IDENT - 1
#define TOK(id, str) ,id
  #include "include/toks.h"
#undef TOK
  ,t_ident
  ,t_string_literal
  ,t_wide_string_literal    /*  L"foo"  */
  ,t_utf8_string_literal    /*  u8"foo" */
  ,t_utf16_string_literal   /*  u"foo"  */
  ,t_utf32_string_literal   /*  U"foo"  */
  ,t_numeric_const
} token_type_t;

#define CH_EOB  '\\'  /* end of buffer or '\0' char in file */

// LUT.
typedef struct {
    const char *str;
    token_type_t id;
} token_kw_t;

#define TOK(id, str) {str, id},
static token_kw_t token_str[] = {
  #include "include/toks.h"
#undef TOK
  {"identifier", t_ident},
  {"string_literal", t_string_literal},
  {"wide_string_literal", t_wide_string_literal},
  {"utf8_string_literal", t_utf8_string_literal},
  {"utf16_string_literal", t_utf16_string_literal},
  {"utf32_string_literal", t_utf32_string_literal},
  {"numeric_const", t_numeric_const},
};

/* Single Token */
typedef struct token {
  token_type_t tok;
  char *str; 
  /* length of the token ptr. fat pointer implemenation. */
  int ptr_len;
  /* line number */
  int line_num;
} token_t;

/* Linked List of Tokens */
typedef struct tokens {
  token_t tok;
  struct tokens * next;
} tokens_t;

/* Compiler state */
typedef struct cc_state {
  /* command line options */
  unsigned char verbose;
  unsigned char dump_ast;
  unsigned char dump_tokens;

  unsigned char filetype; 

  /* error handling */
  jmp_buf err_jmp_buf;
  int nb_errors;
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

/* hsccpp.c
 *
 * token helpers 
 * */
void show_tokens(tokens_t *tokens);
void stdout_token(token_t *token);
const char* get_token_str(int id);
token_type_t get_tok_expr(const char *str, int ptr_len);
token_t *get_token(cc_state_t *s);

tokens_t *cc_lex_analysis(cc_state_t *s);
/* Runs the preprocessor pipeline. */
void cc_preprocess(cc_state_t *s);

#endif
