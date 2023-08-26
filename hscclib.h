/*
 *  HSCC C Compiler Library
 * 
*/

#ifndef HSCCLIB_H
#define HSCCLIB_H

#include "hscc.h"

/* init compilation program context */
cc_state *cc_init(void);

/* free the compilation context */
void cc_delete(cc_state *s);

#endif
