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

char *keyword_str [] = {
  "auto", 
  "double", 
  "int", 
  "struct",
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
  ident = 0,
  keyword = 1,
  integer = 2,
  str = 3,
  schar = 4,
  ops = 5,
} token_type_t;

typedef struct token {
  token_type_t t;
  char *s; 
  int ptr_l;
} token_t;

const char * const token_str[] =
{
  [keyword] = "KEYWORD",
  [ident] = "IDENT",
  [integer]  = "INTEGER",
  [str]  = "STRING",
  [schar]  = "SPECIAl CHAR",
  [ops]  = "OPERATOR",
};

void stdout_token(token_t *token) {
  printf("  [(%s, %d), %.*s]\n", token_str[token->t], token->t, token->ptr_l, token->s);
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
          t  = (token_t) { .t = schar, .s = &s[i], .ptr_l = 1 };
          append_token(&tokens, t);
        } else if ( s[i] == '"' ) 
        {
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

              if (strin(keyword_str, 32, dest)) {
                  t = (token_t) { .t = keyword, .s = &s[i - lc], .ptr_l = lc };
              } else {
                  t = (token_t) { .t = ident, .s = &s[i - lc], .ptr_l = lc };
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
