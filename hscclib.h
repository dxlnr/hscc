/*
 *  HSCC C Compiler: Library
 * 
*/

#ifndef HSCCLIB_H
#define HSCCLIB_H

#include "hscc.h"

int parse_args(cc_state_t *s, int *argc, char ***argv);
/* get the file extension */
char *cc_get_file_ext(const char *fn);
/* write a file into a buffer */
void write_file_to_buf(cc_state_t *s, const char *fn, int len);
/* adds a file to compilation. */
int cc_add_file(cc_state_t *s, const char *fn);
/* init compilation program context */
cc_state_t *cc_init(void);
/* free the compilation context */
void cc_delete(cc_state_t *s);

/* Compile a C source file. 
 *
 * Main function for the actual compiler:
 * preprocessor -> parser -> codegen.
 */
int cc_compile(cc_state_t *s, const char *str, int fd);

#endif
