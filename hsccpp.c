/*
 *  HSCC: C Compiler: Preprocessing and Lexing.
 * 
*/

#include "hscc.h"

void stdout_token(token_t *token) {
  if (token != NULL)
    /* printf("  %-16s '%.*s'\n", token_str[token->tok], token->ptr_len, token->str); */
    printf("  %-16d '%.*s'\n", token->tok, token->ptr_len, token->str);
}

void show_tokens(tokens_t *tokens) {
  while (tokens != NULL) {
    stdout_token(&tokens->tok);
    tokens = tokens->next;
  }
}

const char * const token_str[] = {
  #define TOK(id, str) str,
    #include "include/toks.h"
  #undef TOK
};

/* find a token and add it to table. */
token_t *tok_add(const char *str, int len)
{
}

/* finds a tokens str. */
const char *get_tok_str(int v)
{
}

void append_token(tokens_t **head_ref, token_t *token) {
  tokens_t * n_token = (tokens_t *) malloc(sizeof(tokens_t));
  n_token->tok = *token;

  tokens_t* last = *head_ref;
  n_token->next = NULL;

  if (*head_ref == NULL) {
    *head_ref = n_token;
    return;
  }
  while (last->next != NULL) {
    last = last->next;
  }
  last->next = n_token;
}

void run_preprocessing(int tok)
{
  switch(tok) {
    case t_define:
    case t_include:
    case t_include_next:
    case t_ifdef:
    case t_ifndef:
    case t_elif:
    case t_endif:
    case t_defined:
    case t_undef:
    case t_error:
    case t_warning:
    case t_line:
    case t_pragma:
    default:
      break;
    }
}

uint8_t *parse_comment(uint8_t *p){
}


token_t* assign_tok(token_type_t tok, char *str, int len) {
  token_t *token = (token_t *) malloc(sizeof(token_t));
  if (token == NULL)
    exit(1);
  token->tok = tok;
  token->str = str;
  token->ptr_len = len;

  return token;
}

static int get_next_ch(file_buffer_t *fb)
{
  int ch = *fb->buf_ptr;
  /* end of buffer/file handling */
  if (ch == CH_EOB || fb->buf_ptr >= fb->buf_end) {
    return -1;
  }
  fb->buf_ptr++;
  return ch;
}

token_t *get_next_token(file_buffer_t *file) {
  int c;

  uint8_t *p, *pstart;
  p = file->buf_ptr;
  pstart = p;
  
redo_start:
  c = *p;
  switch (c) {
    case ' ':
    case '\t':
    case '\f':
    case '\v':
    case '\r':
    case '\\':
      break;
    case '\n':
      file->line_num++;
      break;
    case '#':
      printf("hash\n");
      p++;
      if (*p == '#') {
        return assign_tok(hashhash, (char *) pstart, 2);
      } else {
        return assign_tok(hash, (char *) pstart, 1);
      }
      break;

    case '(':
    case ')':
    case '[':
    case ']':
    case '{':
    case '}':
    case ',':
    case ';':
    case ':':
    case '?':
    case '~': 

    case '$':

    case 'a': case 'b': case 'c': case 'd':
    case 'e': case 'f': case 'g': case 'h':
    case 'i': case 'j': case 'k': case 'l':
    case 'm': case 'n': case 'o': case 'p':
    case 'q': case 'r': case 's': case 't':
    case 'u': case 'v': case 'w': case 'x':
    case 'y': case 'z': 
    case 'A': case 'B': case 'C': case 'D':
    case 'E': case 'F': case 'G': case 'H':
    case 'I': case 'J': case 'K': 
    case 'M': case 'N': case 'O': case 'P':
    case 'Q': case 'R': case 'S': case 'T':
    case 'U': case 'V': case 'W': case 'X':
    case 'Y': case 'Z': 
    case '_': 

    case 'L':

    case '0': case '1': case '2': case '3':
    case '4': case '5': case '6': case '7':
    case '8': case '9':

    case '.':

    case '\'':
    case '\"':

    case '<':
    case '>':
    case '&':
    case '|':
    case '+':
    case '-':

    /* comments or operator */
    case '/':

    default:
      printf("(char) %c\n", c);
      p++;
      break;
  }
  return NULL;
}

void cc_preprocess(cc_state_t *s) {
  tokens_t *tokens;
  token_t *tok;

  for (;;) {
    tok = get_next_token(s->fb);

    if (tok != NULL)
      append_token(&tokens, tok);

    int c = get_next_ch(s->fb);
    if (c == -1) {
      printf("EOF\n");
      break;
    }
  }
  show_tokens(tokens);

}
