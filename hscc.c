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
    "  -E                  Output the preprocessing results .\n"
;

const char *default_out(cc_state_t *s, const char *ffile)
{
  char buf[1024];
  char *ext;
  const char *name = "a";
  return name;
}

int main(int argc, char **argv)
{
  cc_state_t *state;
  const char *ff;
  int fd, ret, done, n = 0;

  state = cc_init();

  int args = parse_args(state, &argc, &argv);
  if (args < 0) return 1;

  if (args == ARG_HELP){
    fputs(show_help, stdout);
    return 0;
  }
  if (state->fc == 0) return 1;

  ff = NULL, ret = 0;
  do {
    file_spec_t *f = state->files[n];
    state->filetype = f->type;
    if (state->verbose == 1)
      printf("compiling -> %s\n", f->name);
    if (!ff)
      ff = f->name;
    fd = cc_add_file(state, f->name);
    if (fd < 0)
      ret = 1;

    ret = cc_compile(state, ff, fd); 
    done = ret || ++n >= state->fc;
  } while (!done && (state->output_type != CC_OUTPUT_OBJ ));


  return 0;
}
