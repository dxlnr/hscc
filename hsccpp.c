/*
 *  HSCC: C Compiler: Preprocessing and Lexing.
 * 
*/

#include "hscc.h"

void stdout_token(token_t *token) {
  printf("  %-16s '%.*s'\n", token_str[token->tok], token->ptr_len, token->str);
}

const char * const token_str[] = {
  #define TOK(id, str) str,
    #include "include/toks.h"
  #undef TOK
};

token_type_t get_token_type_from_str(char *c, int len) 
{
  for (int i = 0; i < sizeof(token_str) / sizeof(token_str[0]); i++) {
    if (strncmp(c, token_str[i], len) == 0 && strlen(token_str[i]) == len) {
      printf("token: %s | %s\n", token_str[i], c);
      return t_error;
    }
  }
  return t_error;
}

typedef enum state {
  START = 0,
  DIGIT = 1,
  LETTER = 2, 
  OPS = 3,
  STR = 4,
  ERROR = 5,
} state_t;

/* find a token and add it to table. */
token_t *tok_add(const char *str, int len)
{
}

/* finds a tokens str. */
const char *get_tok_str(int v)
{
}

void append_token(tokens_t **head_ref, token_t token) {
  tokens_t * n_token = (tokens_t *) malloc(sizeof(tokens_t));
  n_token->tok = token;

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

tokens_t *cc_lex_analysis(cc_state_t *s) {
  token_t tok;
  tokens_t *tokens;
  state_t l_state = START;

  int i = 0;
  int dc = 0;
  int lc = 0;
  int ops_c = 0;
  int str_c = 0;

  /* for (int i = 0; i < strlen(s); ++i) { */
  while (s->fb->buf_ptr < s->fb->buf_end) {
    switch(l_state) {
      case START:
        if (isdigit(s->fb->buf_ptr[0])) {
          l_state = DIGIT;
          dc++;
        } else if (isalpha(s->fb->buf_ptr[0])) {
          l_state = LETTER;
          lc++;
        } else if (strchr(TOK_OPERATORS, s->fb->buf_ptr[0])) {
          l_state = OPS;
          ops_c++;
        } else if (strchr(TOK_DELIMITERS, s->fb->buf_ptr[0])) {
          tok  = (token_t) { .tok = get_token_type_from_str((char *)s->fb->buf_ptr, 1), .str = (char *) s->fb->buf_ptr, .ptr_len = 1 };
          append_token(&tokens, tok);
        } else if ( s->fb->buf_ptr[0] == '"' ) {
          l_state = STR;
          str_c++;
        }               
        printf("state : %d\n", l_state);
        break;
      case DIGIT:
        if (isdigit(s->fb->buf_ptr[0])) {
          dc++;
        } else {
          /* tok = (token_t) { .tok = t_numeric_const, .str = (char *) s->fb->buf_ptr[i - dc], .ptr_len = dc }; */
          /* append_token(&tokens, tok); */
          l_state = START;
          dc = 0;
          i--;
        }
        break;
      case LETTER:
        if (isalnum(s->fb->buf_ptr[0]) || s->fb->buf_ptr[0] == '_') {
          lc++;
        } else {
          /* tok = (token_t) { .tok = get_token_type_from_str(&s[i - lc], lc), .str = &s[i - lc], .ptr_len = lc }; */
          /* append_token(&tokens, tok); */
          l_state = START;
          lc = 0;
          i--;
        }
        break;
      case OPS:
        if (strchr(TOK_OPERATORS, s->fb->buf_ptr[0])) {
          ops_c++;
        } else {
          /* tok = (token_t) { .tok = get_token_type_from_str(&s[i - ops_c], ops_c), .str = &s[i - ops_c], .ptr_len = ops_c }; */
          /* append_token(&tokens, tok); */
          l_state = START;
          ops_c = 0;
          i--;
        }
        break;
      case STR:
        if (( s->fb->buf_ptr[0] != '"' ) && ( s->fb->buf_ptr[0] != '\n' )) 
        {
          str_c++;
        } else {
          /* tok = (token_t) { .tok = t_string_literal, .str = &s[i - str_c], .ptr_len = str_c + 1 }; */
          /* append_token(&tokens, tok); */
          l_state = START;
          str_c = 0;
        }
        break;
      default:
        l_state = START;
      }
      printf("%c\n", s->fb->buf_ptr[0]);
      s->fb->buf_ptr++;
      i++;
    }
    return tokens;
}

void show_tokens(tokens_t *tokens) {
  while (tokens != NULL) {
    stdout_token(&tokens->tok);
    tokens = tokens->next;
  }
}

void cc_preprocess(cc_state_t *s) {

  /* while (s->fb->buf_ptr < s->fb->buf_end) { */
  /*   /1* printf("%s\n", s->fb->buf_ptr); *1/ */
  /*   s->fb->buf_ptr++; */
  /* } */
  tokens_t *tokens = cc_lex_analysis(s);
}
