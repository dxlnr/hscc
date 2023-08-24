#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *read_file(char *fname) 
{
  FILE *file = NULL;
  file = fopen(fname, "r");

  if (file == NULL) return NULL;

  fseek(file, 0, SEEK_END);
  int length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *s = malloc(sizeof(char) * (length + 1)); 
  char c;
  int i = 0;
  while( (c = fgetc(file)) != EOF)
  {
      s[i] = c;
      i++;
  }
  s[i] = '\0';

  fclose(file);

  return s;
}

char* substr(const char *src, int m, int n)
{
  int len = n-m;
  char *dest = (char*) malloc(sizeof(char)*(len + 1));
  for (int i=m; i<n && (*(src+i) != '\0'); i++)
  {
      *dest = *(src + i);
      dest++;
  }
  *dest = '\0';
  return (dest - len);
}

int strin(char **arr, int len, char *s) {
  for(int i = 0; i < len; ++i) {
    if(strncmp(arr[i], s, strlen(s)) == 0) {
      return 1;
    }
  }
  return 0;
}

typedef enum states {
  START = 0,
  DIGIT = 1,
  LETTER = 2, 
  OPS = 3,
  STR = 4,
} states_t;

char *kw_str[] = {
  "auto", "double", "int", "struct",
  "break", 
  "else",  
  "long",
  "switch",
  "case",
  "enum",
  "register",  
  "typedef",
  "char", 
  "extern",     
  "return", 
  "union",
  "const",
  "float",       
  "short",      
  "unsigned",
  "continue",  
  "for",         
  "signed",   
  "void",
  "default",      
  "goto",        
  "sizeof",     
  "volatile",
  "do",         
  "if", 
  "static",    
  "while",
};

typedef enum token_type {
  t_ident, 
  integer, 
  str, 
  ops,

  t_auto, t_int, t_double, t_struct, t_break, t_else, t_long, t_switch, t_case, t_enum, 
  t_register, t_typedef, t_char, t_extern, t_return, t_union, t_const, t_float, t_short, 
  t_unsigned, t_continue, t_for, t_signed, t_void, t_default, t_goto, t_sizeof, t_volatile, 
  t_do, t_if, t_static, t_while,

  t_arrow, t_colon, t_comma, t_dot, t_equal, t_equal_arrow, t_semicolon,
  t_less,
  t_greater,
  t_l_brace,
  t_r_brace,
  t_l_paren,
  t_r_paren,
  t_l_square,
  t_r_square,
  t_underscore,
  t_directive,
  t_integer,
  t_string_block,
  t_string,
  t_eof, 

  t_error,

  t_pphash,
} token_type_t;

const static struct {
    token_type_t val;
    const char *str;
} kw_to_token [] = {
    {t_auto, "auto"},
    {t_int, "int"},
    {t_double, "double"},
    {t_struct, "struct"},
    {t_break, "break"}, 
    {t_else, "else"},  
    {t_long, "long"},
    {t_switch, "switch"},
    {t_case, "case"},
    {t_enum, "enum"},
    {t_register, "register"},  
    {t_typedef, "typedef"},
    {t_char, "char"}, 
    {t_extern, "extern"},     
    {t_return, "return"},    
    {t_union, "union"},
    {t_const, "const"},
    {t_float, "float"},       
    {t_short, "short"},      
    {t_unsigned, "unsigned"},
    {t_continue, "continue"},  
    {t_for, "for"},         
    {t_signed, "signed"},   
    {t_void, "void"},
    {t_default, "default"},      
    {t_goto, "goto"},        
    {t_sizeof, "sizeof"},     
    {t_volatile, "volatile"},
    {t_do, "do"},         
    {t_if, "if"}, 
    {t_static, "static"},    
    {t_while, "while"},
};

token_type_t kw_to_token_type (const char *str, int len)
{
  for (int i = 0;  i < sizeof (kw_to_token) / sizeof (kw_to_token[0]);  ++i)
    if (strncmp(str, kw_to_token[i].str, len) == 0) {
      return kw_to_token[i].val;    
    }
  return t_error;
}

typedef struct token {
  token_type_t t;
  char *s; 
  int ptr_l;
} token_t;

const char * const token_str[] =
{
  [t_ident]    = "identifier",
  [integer] = "INTEGER",
  [str] = "STRING",

  [t_auto]     = "auto",
  [t_int]      = "int",
  [t_double]   = "double",
  [t_struct]   = "struct",
  [t_break]    = "break", 
  [t_else]     = "else",  
  [t_long]     = "long",
  [t_switch]   = "switch",
  [t_case]     = "case",
  [t_enum]     = "enum",
  [t_register] = "register",  
  [t_typedef]  = "typedef",
  [t_char]     = "char", 
  [t_extern]   = "extern",     
  [t_return]   = "return",    
  [t_union]    = "union",
  [t_const]    = "const",
  [t_float]    = "float",
  [t_short]    = "short",
  [t_unsigned] = "unsigned",
  [t_continue] = "continue",  
  [t_for]      = "for",         
  [t_signed]   = "signed",   
  [t_void]     = "void",
  [t_default]  = "default",      
  [t_goto]     = "goto",        
  [t_sizeof]   = "sizeof",     
  [t_volatile] = "volatile",
  [t_do]       = "do",         
  [t_if]       = "if", 
  [t_static]   = "static",    
  [t_while]    = "while",

  [ops]  = "OPERATOR",
  [t_colon] = "colon",
  [t_comma] = "comma",
  [t_dot] = "dot",
  [t_semicolon] = "semi",
  [t_l_brace] = "l_brace",
  [t_r_brace] = "r_brace",
  [t_l_paren] = "l_paren",
  [t_r_paren] = "r_paren",
  [t_l_square] = "l_square",
  [t_r_square] = "r_square",
  [t_eof] = "eof", 

  [t_pphash] = "hash", 
};

void stdout_token(token_t *token) {
  printf("  %-10s '%.*s'\n", token_str[token->t], token->ptr_l, token->s);
}

typedef struct tokens {
  token_t t;
  struct tokens * next;
} tokens_t;

void append_token(tokens_t **head_ref, token_t token) {
  tokens_t * n_token = (tokens_t *) malloc(sizeof(tokens_t));
  n_token->t = token;

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

void push(tokens_t **head_ref, token_t token)
{
  tokens_t * n_token = (tokens_t *) malloc(sizeof(tokens_t));
  n_token->t = token;
  n_token->next = (*head_ref);
  (*head_ref) = n_token;
}

void show_tokens(tokens_t *tokens) {
  while (tokens != NULL) {
    stdout_token(&tokens->t);
    tokens = tokens->next;
  }
}

token_type_t get_token_type_schar(char c) {
  switch (c) {
      case '[': return t_l_square;
      case ']': return t_r_square;
      case '{': return t_l_brace;
      case '}': return t_r_brace;
      case '(': return t_l_paren;
      case ')': return t_r_paren;
      case ',': return t_comma;
      case ';': return t_semicolon;
      case ':': return t_colon;
      case '.': return t_dot;
      case '#': return t_pphash;
      default:  return t_error;
  }
}

tokens_t *lexical_analysis(char *s) {
  token_t t;
  tokens_t *tokens = NULL;
  states_t state = START;

  int dc = 0;
  int lc = 0;
  int ops_c = 0;
  int str_c = 0;

  for (int i = 0; i < strlen(s); ++i) {
    switch(state) {
      case START:
        if (isdigit(s[i])) {
          state = DIGIT;
          dc++;
        } else if (isalpha(s[i])) {
          state = LETTER;
          lc++;
        } else if ( 
          ( s[i] == '+' ) || ( s[i] == '-' ) ||
          ( s[i] == '/' ) || ( s[i] == '*' ) ||
          ( s[i] == '%' ) || ( s[i] == '=' ) ||
          ( s[i] == '<' ) || ( s[i] == '>' ) ||
          ( s[i] == '&' ) || ( s[i] == '^' ) ||
          ( s[i] == '|' ) || ( s[i] == '!' ))
        {
          state = OPS;
          ops_c++;
        } else if (
          ( s[i] == '[' ) || ( s[i] == ']' ) || 
          ( s[i] == '(' ) || ( s[i] == ')' ) ||
          ( s[i] == '{' ) || ( s[i] == '}' ) ||
          ( s[i] == ',' ) || ( s[i] == ';' ) || 
          ( s[i] == ':' ) || ( s[i] == '~' ) ||
          ( s[i] == '.' ) || ( s[i] == '#' )) 
        {
          t  = (token_t) { .t = get_token_type_schar(s[i]), .s = &s[i], .ptr_l = 1 };
          append_token(&tokens, t);
        } else if ( s[i] == '"' ) {
          state = STR;
          str_c++;
        }               
        break;
      case DIGIT:
        if (isdigit(s[i])) {
          dc++;
        } else {
          t = (token_t) { .t = integer, .s = &s[i - dc], .ptr_l = dc };
          append_token(&tokens, t);
          state = START;
          dc = 0;
          i--;
        }
        break;
      case LETTER:
        if ( isalpha(s[i]) || isdigit(s[i]) || ( s[i] == '_' ) ) {
          lc++;
        } else {
              char* dest = substr(s, i - lc, i);
              if (strin(kw_str, 32, dest)) {
                  t = (token_t) { .t = kw_to_token_type(&s[i - lc], lc), .s = &s[i - lc], .ptr_l = lc };
              } else {
                  t = (token_t) { .t = t_ident, .s = &s[i - lc], .ptr_l = lc };
              }
              append_token(&tokens, t);
              state = START;
              lc = 0;
              i--;
        }
        break;
      case OPS:
        if ( 
          ( s[i] == '+' ) || ( s[i] == '-' ) ||
          ( s[i] == '/' ) || ( s[i] == '*' ) ||
          ( s[i] == '%' ) || ( s[i] == '=' ) ||
          ( s[i] == '<' ) || ( s[i] == '>' ) ||
          ( s[i] == '&' ) || ( s[i] == '^' ) ||
          ( s[i] == '|' ) || ( s[i] == '!' ))
        {
          ops_c++;
        } else {
          token_t t = { .t = ops, .s = &s[i - ops_c], .ptr_l = ops_c };
          append_token(&tokens, t);
          state = START;
          ops_c = 0;
          i--;
        }
        break;
      case STR:
        if (( s[i] != '"' ) && ( s[i] != '\n' )) 
        {
          str_c++;
        } else {
          token_t t = { .t = str, .s = &s[i - str_c], .ptr_l = str_c + 1 };
          append_token(&tokens, t);
          state = START;
          str_c = 0;
        }
        break;
      default:
        state = START;
      }
    }
    return tokens;
}

typedef enum ast_type {
  AST_NUMBER,
  AST_ADD,
  AST_MUL,

} ast_type_t;


int main(int argc, char **argv)
{
  char *fc = read_file(argv[1]);
  if (fc == NULL)
  {
      printf("Error reading file.\n");
      return 1;
  }

  tokens_t *tokens = lexical_analysis(fc);

  // Show the results to the console.
  printf("Tokens: \n");
  show_tokens(tokens);

  free(fc);

  return 0;
}
