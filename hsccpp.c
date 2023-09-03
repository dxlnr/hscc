/*
 *  HSCC: C Compiler: Preprocessing and Lexing.
 * 
*/

#include "hscc.h"

const char * const token_str[] = {
  #define TOK(id, str) str,
    #include "include/toks.h"
  #undef TOK
};

/* find a token and add it to table. */
t_tokens_t *tok_add(const char *str, int len)
{
}

/* finds a tokens str. */
const char *get_tok_str(int v)
{
}

int cc_rm_comments() {
  return 0;
}

void cc_preprocess(cc_state_t *s) {
  file_buffer_t *fb = s->fb;

  printf("Preprocessing: %s\n", &s->fb->fn[0]);

  printf("Preprocessing: %s\n", fb->buf_ptr);
  printf("Preprocessing: %s\n", fb->buf_end);
  printf("Preprocessing: %s\n", &fb->buf[0]);
}
