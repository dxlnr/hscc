/*
 *  HSCC C Compiler Library
 * 
*/

#ifndef HSCCLIB_H
#define HSCCLIB_H

#include "hscc.h"

int parse_args(cc_state_t *s, int *argc, char ***argv);

/* init compilation program context */
cc_state_t *cc_init(void);

/* free the compilation context */
void cc_delete(cc_state_t *s);

/* compile a C source file */
int cc_compile(cc_state_t *s, const char *fn);

#endif
