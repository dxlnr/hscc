/*
 *  HSCC C Compiler 
 * 
*/

#include "hscc.h"

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

int main(int argc, char **argv)
{
  char *fc = read_file(argv[1]);
  if (fc == NULL)
  {
      printf("Error reading file.\n");
      return 1;
  }
  return 0;
}
