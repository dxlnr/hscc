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

typedef enum Token {
    ident = 0,
    keyword = 1,
    constant = 2,
    str = 3,
    schar = 4,
    ops = 5,
} token;

/* typedef struct LexerToken { */
/*     token t; */
/*     char *s; */ 
/* }; */

void *lexical_analysis(char *s, token *ts) {
    int i = 0;
    ts = (token*) malloc(sizeof(token) * 1000);

    for (char c = *s; c != '\0'; c = *++s)
    {
        putchar(c);
        if (
           ( c == '[' ) || ( c == ']' ) || 
           ( c == '(' ) || ( c == ')' ) ||
           ( c == '{' ) || ( c == '}' ) ||
           ( c == ',' ) || ( c == ';' ) || 
           ( c == ':' ) || ( c == '=' ) ||
           ( c == '*' ) || ( c == '#' ) ||
           ( c == '.' ) || ( c == '~' ) )
        {
            ts[i] = schar;
            i++;
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

    token* ts;
    lexical_analysis(fc, ts);
    
    for(int i = 0; i < sizeof(ts); i++) { 
        printf("%d ", ts[i]);
    }

    free(fc);

    return 0;
}
