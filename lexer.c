#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef enum token_type {
    ident = 0,
    keyword = 1,
    constant = 2,
    str = 3,
    schar = 4,
    ops = 5,
} token_type_t;

typedef struct token {
    token_type_t t;
    char *s; 
    int ptr_l;
} token_t;

void stdout_token(token_t *token) {
    printf("  (%.*s, %d)\n", token->ptr_l, token->s, token->t);
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

void lexical_analysis(char *s, tokens_t *ts) {
    for (int i = 0; i < strlen(s); ++i){
        if (
           ( s[i] == '[' ) || ( s[i] == ']' ) || 
           ( s[i] == '(' ) || ( s[i] == ')' ) ||
           ( s[i] == '{' ) || ( s[i] == '}' ) ||
           ( s[i] == ',' ) || ( s[i] == ';' ) || 
           ( s[i] == ':' ) || ( s[i] == '=' ) ||
           ( s[i] == '*' ) || ( s[i] == '#' ) ||
           ( s[i] == '.' ) || ( s[i] == '~' ) )
        {
            token_t t = { .t = schar, .s = &s[i], .ptr_l = 1 };
            append_token(&ts, t);
        }
    }
}

 
int main(int argc, char **argv)
{
    char *fc = read_file(argv[1]);
    if (fc == NULL)
    {
        printf("Error reading file.\n");
        return 1;
    }

    tokens_t *tokens = NULL;
    token_t test = { .t = schar, .s = "", .ptr_l = 1 };
    push(&tokens, test);
    lexical_analysis(fc, tokens);

    printf("Tokens: \n");
    show_tokens(tokens);
    
    free(fc);

    return 0;
}
