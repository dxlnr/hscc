/*
 *  HSCC C Compiler 
 * 
*/

#include "hscc.h"
#include "hscclib.h"

static const char show_help[] =
    "HSCC C Compiler "" \n"
    "\n"
    "Usage: hscc [options...] [-o outfile] [-c] input file(s)...\n"
    "General options:\n"
    "  -o <file>           Place the output into <file>.\n"
    "  -h                  Display available options (-help-hidden for more).\n"
    "  -verbose            Display verbose messages.\n"
;

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
  cc_state *state;
  int ret;

  state= cc_init();

  int args = parse_args(state, &argc, &argv);
  if (args < 0) return 1;

  if (args == ARG_HELP){
    fputs(show_help, stdout);
    return 0;
  }
  /* char *fc = read_file(argv[1]); */
  if (state->fc == 0) return 1;

  /* cc_compile(cs, fc); */

  return 0;
}
