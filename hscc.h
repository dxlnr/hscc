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


/* assignment operators */
#define T_A_ADD   0x0
#define T_A_SUB   0x1
#define T_A_MUL   0x2
#define T_A_DIV   0x3
#define T_A_MOD   0x4
#define T_A_AND   0x5
#define T_A_OR    0x6
#define T_A_XOR   0x7
#define T_A_SHL   0x8
#define T_A_SAR   0x9

#define T_ASSIGN(t) (t >= TOK_A_ADD && t <= TOK_A_SAR)
#define T_ASSIGN_OP(t) ("+-*/%&|^<>"[t - T_A_ADD])
#define T_IDENT 256

enum hscc_token_type {
    T_LAST = T_IDENT - 1
#define DEF(id, str) ,id
#include "toks.h"
#undef DEF
};

/* Compiler state */
typedef struct cc_state {
  unsigned char verbose;
} cc_state;

#endif
