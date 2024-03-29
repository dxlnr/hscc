/*
 *  HSCC: C Compiler: Preprocessing and Lexing.
 *
*/

#include "hscc.h"

const char *get_tok_str(int tok) 
{
  for (int i = 0; token_str[i].id != sizeof(token_str) / sizeof(token_str[0]); i++) {
    if (token_str[i].id == tok) {
      return token_str[i].str;
    }
  }
  return NULL;
}

token_type_t get_tok_expr(const char *str, int ptr_len) 
{
  for (int i = 0; token_str[i].str != NULL; i++) {
    if (strncmp(token_str[i].str, str, ptr_len) == 0) {
      return token_str[i].id;
    }
  }
  return t_eof;
}

void stdout_token(token_t *token) 
{
  if (token != NULL)
    printf("  %-20s %-25.*s %d\n", 
           get_tok_str(token->tok), 
           token->ptr_len, 
           token->str, 
           token->line_num);
}

void show_tokens(tokens_t *tokens) 
{
  while (tokens != NULL) {
    stdout_token(&tokens->tok);
    tokens = tokens->next;
  }
}

/* find a token and add it to table. */
token_t *tok_add(const char *str, int len)
{
}

void append_token(tokens_t **head_ref, token_t *token) 
{
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

void free_tokens_list(tokens_t* head) 
{
  tokens_t* current_tok = head;
  tokens_t* next_tok = NULL;

  while (current_tok != NULL) {
    next_tok = current_tok->next; 
    free(current_tok);
    current_tok = next_tok;
  }
}

token_t *next_token(tokens_t **toks) 
{
  tokens_t* ts = *toks;

  if (ts->next != NULL) {
    *toks = ts->next;
    return &ts->tok;
  }
  return NULL;
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
      break;
    default:
      break;
    }
}

uint8_t *parse_binary_ops(uint8_t *p)
{
}

int is_kw_token(char *str, int len) 
{
  for (int i = 0; i <= N_KEYWORDS; i++) {
    if (strncmp(token_str[i].str, str, len) == 0) {
      return 1;
    }
  }
  return 0;
}

token_t* assign_tok(token_type_t tok, char *str, int len, int line_num) 
{
  token_t *token = (token_t *) malloc(sizeof(token_t));

  if (token == NULL)
    exit(1);
  token->tok = tok;
  token->str = str;
  token->ptr_len = len;
  token->line_num = line_num;

  return token;
}

static int get_next_ch(file_buffer_t *fb)
{
  int ch = *fb->buf_ptr;
  /* end of buffer/file handling */
  if (ch == CH_EOB || fb->buf_ptr >= fb->buf_end) {
    cc_close_buf(fb);
    return -1;
  }
  fb->buf_ptr++;
  return ch;
}

token_t *find_next_token(file_buffer_t *file) 
{
  int c; 
  int tl;
  token_t *t;
  uint8_t *p; 
  uint8_t *pstart;

  p = file->buf_ptr;
  pstart = p;
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
      p++;
      if (*p == '#') {
        p++;
        return assign_tok(hashhash, (char *) pstart, 2, file->line_num);
      } else {
        return assign_tok(hash, (char *) pstart, 1, file->line_num);
      }
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
      return assign_tok(get_tok_expr((char *) pstart, 1), 
                        (char *) pstart, 1, file->line_num);
    case '$':
      break;

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
    is_identifier:
      p++;
      if (isalnum(*p) || *p == '_')
        goto is_identifier;

      file->buf_ptr = --p;

      if (is_kw_token((char *) pstart, p - pstart + 1))
        return assign_tok(get_tok_expr((char *) pstart, p - pstart + 1), 
                          (char *) pstart, p - pstart + 1, file->line_num);
      else
        return assign_tok(t_ident, 
                          (char *) pstart, p - pstart + 1, file->line_num);
    case 'L':
      p++;
      if (*p == '\'' || *p == '\"') {
        goto is_string_const;
      } else {
        goto is_identifier;
      }

    case '0': case '1': case '2': case '3':
    case '4': case '5': case '6': case '7':
    case '8': case '9':
      if (c == '0') {
        p++;
        if (*p == 'x' || *p == 'X' ||
            *p == 'b' || *p == 'B' || 
            *p == 'b' || *p == 'B' ||
            *p == 'e' || *p == 'E' ||
            *p == '.') {
          goto is_number;
        } else if (*p == ' ') {
          return assign_tok(t_numeric_const, 
                            (char *) pstart, p - pstart, file->line_num);
        } else {
          return assign_tok(t_numeric_const, 
                            (char *) pstart, p - pstart, file->line_num);
        }
      }     
    is_number:
      p++;
      if (isdigit(*p))
        goto is_number;

      file->buf_ptr = --p;
      return assign_tok(t_numeric_const, 
                        (char *) pstart, p - pstart + 1, file->line_num);
    case '.':
      return assign_tok(period, 
                        (char *) pstart, 1, file->line_num);

    case '\'':
    case '\"':
    is_string_const:
      p++;
      if (*p != '\'' && *p != '\"') {
        goto is_string_const;
      }

      file->buf_ptr = p;
      if (*pstart == 'L') {
        return assign_tok(t_wide_string_literal, 
                          (char *) pstart, p - pstart + 1, file->line_num);
      } else {
        return assign_tok(t_string_literal, 
                          (char *) pstart, p - pstart + 1, file->line_num);  
      }

    case '<':
    case '>':
      p++;
      if (*p == c) {
        p++;
        file->buf_ptr = p;
        if (*p == '=') {
          tl = 3;
        } else tl = 2;
      } else if (*p == '=') {
        file->buf_ptr = p;
        tl = 2;
      } else tl = 1;
      return assign_tok(get_tok_expr((char *) pstart, tl) , 
                                     (char *) pstart, tl, file->line_num);
    case '&':
    case '|':
    case '+':
      p++;
      if (*p == c || *p == '=') {
        file->buf_ptr = p;
        tl = 2;
      } else tl = 1;
      return assign_tok(get_tok_expr((char *) pstart, tl) , 
                                     (char *) pstart, tl, file->line_num);
    case '-':
      p++;
      if (*p == c || *p == '=' || *p == '>') {
        file->buf_ptr = p;
        tl = 2;
      } else tl = 1;
      return assign_tok(get_tok_expr((char *) pstart, tl) , 
                                     (char *) pstart, tl, file->line_num);
    case '*':
    case '%':
    case '^':
    case '!':
    case '=':
    is_div_operator:
      p++;
      if (*p == '=') {
        file->buf_ptr = p;
        tl = 2;
      } else tl = 1;
      return assign_tok(get_tok_expr((char *) pstart, tl) , 
                                     (char *) pstart, tl, file->line_num);
    /* comments or operator */
    case '/':
      p++;
      if (*p == '/' || *p == '*') {
        p++;
        while (*p != '\n' && *p != '\0') {
          p++;
        }
        file->buf_ptr = p;
        break;
      } else 
        goto is_div_operator;
        
    default:
      p++;
      break;
  }
  return NULL;
}

void cc_parse(tokens_t *toks) 
{
  token_t *t;

next_token:
  t = next_token(&toks);

  switch(t->tok) {
    default:
      break;
  }
}


int cc_preprocess(cc_state_t *s) 
{
  tokens_t *tokens;
  token_t *tok;

  for (;;) {
    tok = find_next_token(s->fb);

    if (tok != NULL) {
      append_token(&tokens, tok);
      /* preprocess token immediately. */
      /* run_preprocessing(tok->tok); */
    }

    int c = get_next_ch(s->fb);
    if (c == -1) {
      if (s->verbose)
        printf("(v) Lexing finished. EOF.\n");
      break;
    }
  }
  if (s->verbose) show_tokens(tokens);
  if (s->dump_tokens) {
    show_tokens(tokens);
    return 0;
  }

  /* parsing */
  for (;;) {
    tok = next_token(&tokens);
    
    if (tok != NULL) {
    } else {
      break;
    }
  }
  if (s->dump_ast) {
    exit(0);
  }

  return 0;
}
