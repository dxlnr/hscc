/*
 *  HSCC C Compiler Library
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
/* runs a file through compilation. */
int cc_run_file(cc_state_t *s, const char *fn);
/* init compilation program context */
cc_state_t *cc_init(void);
/* free the compilation context */
void cc_delete(cc_state_t *s);
/* compile a C source file */
int cc_compile(cc_state_t *s);

#endif
