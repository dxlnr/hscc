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

void show_tokens(tokens_t *tokens) {
  while (tokens != NULL) {
    stdout_token(&tokens->tok);
    tokens = tokens->next;
  }
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

void get_next_token(file_buffer_t *file) {
  int c, tok;
  uint8_t *p;
  p = file->buf_ptr;
  
redo_start:
  c = *p;
  switch (c) {
    case ' ':
    case '\t':
      p++;
      goto redo_start;
    case '\f':
    case '\v':
    case '\r':
      p++;
      goto redo_start;
    case '\\':
      break;
    case '\n':
      file->line_num++;
      p++;
    case '#':
      p++;
      if (*p == '#') {
        tok = hashhash;
        p++;
      } else {
        tok = hash;
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
      p++;

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
      printf("unknow char %c\n", c);
      break;
  }
}

void cc_preprocess(cc_state_t *s) {

  for (;;) {
    int c = get_next_ch(s->fb);
    if (c == -1) {
      printf("EOF\n");
      break;
    }
    printf("%c", c);

    get_next_token(s->fb);
  }

  /* printf("\n %d lines\n", file->line_num); */

  /* while (s->fb->buf_ptr < s->fb->buf_end) { */
  /*   printf("%s\n", s->fb->buf_ptr); */
  /*   s->fb->buf_ptr++; */
  /* } */
  /* tokens_t *tokens = cc_lex_analysis(s); */
}
