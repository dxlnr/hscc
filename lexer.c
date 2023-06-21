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
} token_t;

void print_token(token_t *token) {
    printf("(%s, %d)", token->s, token->t);
}

typedef struct tokens {
    token_t t;
    struct tokens* next;
} tokens_t;

void push(tokens_t *head, token_t token) {
    tokens_t *current = head;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = (tokens_t *) malloc(sizeof(tokens_t));
    current->next->t= token;
    current->next->next = NULL;
}

void show_tokens(tokens_t *head) {
    tokens_t * current = head;

    while (current != NULL) {
        print_token(&current->t);
        current = current->next;
    }
}

void lexical_analysis(char *s, tokens_t *ts) {
    token_type_t token_type;
    /* ts = (token*) malloc(sizeof(token) * 1000); */

    for (char c = *s; c != '\0'; c = *++s)
    {
        /* int a = getchar(c); */
        /* printf("%d", a); */
        if (
           ( c == '[' ) || ( c == ']' ) || 
           ( c == '(' ) || ( c == ')' ) ||
           ( c == '{' ) || ( c == '}' ) ||
           ( c == ',' ) || ( c == ';' ) || 
           ( c == ':' ) || ( c == '=' ) ||
           ( c == '*' ) || ( c == '#' ) ||
           ( c == '.' ) || ( c == '~' ) )
        {
            token_t t = { .t = schar, .s = &c };
            print_token(&t);
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
    tokens = (tokens_t *) malloc(sizeof(tokens_t));

    lexical_analysis(fc, tokens);

    /* show_tokens(tokens); */
    
    /* for(int i = 0; i < sizeof(ts); i++) { */ 
    /*     printf("%d ", ts[i]); */
    /* } */

    free(fc);

    return 0;
}
