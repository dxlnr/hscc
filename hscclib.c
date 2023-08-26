/*
 *  HSCC C Compiler Library Implementation.
 * 
*/
#include "hscc.h"

void dynarray_add(void *pa, int *nb_ptr, void* data)
{
}

void mem_error(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit (1);
}

void *hscc_malloc(unsigned long size)
{
    void *ptr;
    ptr = malloc(size);
    if (!ptr && size)
        mem_error("memory full (malloc).");
    return ptr;
}

static void args_parser_add_file(cc_state_t *s, const char* fn, int filetype)
{
    struct file_spec *f = hscc_malloc(sizeof *f + strlen(fn));
    f->type = filetype;
    strcpy(f->name, fn);
    dynarray_add(&s->files, &s->fc, f);
}

typedef struct arg_options {
  const char *name;
  uint16_t idx;
  uint16_t flags;
} arg_options_t;

enum {
  arg_option_ignored = 0,
  arg_option_help,
  arg_option_verbose,
  arg_option_o,
  arg_option_dump_ast,
  arg_option_dump_tokens,
};

static const arg_options_t args_options [] = {
  { "h",            arg_option_help,        0 },
  { "-verbose",     arg_option_verbose,     0 },
  { "-dump-ast",    arg_option_dump_ast,    0 },
  { "-dump-tokens", arg_option_dump_tokens, 0 },
  { "-o",           arg_option_o,           0 },
  { NULL,           arg_option_ignored,     0 }
};

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

int parse_args(cc_state_t *s, int *argc, char ***argv) {
  int nb_args = *argc;
  int ca = 1;

  while (ca < nb_args) {
    char *arg = (*argv)[ca];

    if (*arg != '-' && *arg != '\0') {
      s->fc++;
    } else {
      if (strcmp(arg, "-verbose") == 0) {
        s->verbose = 1;
      } else if (strcmp(arg, "-dump-ast") == 0) {
        s->dump_ast = 1;
      } else if (strcmp(arg, "-dump-tokens") == 0) {
        s->dump_tokens = 1;
      } else if (strcmp(arg, "-o") == 0) {
        s->outfile = (*argv)[ca+1];
        ++ca;
      } else {
        return -1;
      }
    }
    ++ca;
    return 0;
  }

  printf("fc: %d\n", s->fc);
  printf("outfile: %s\n", s->outfile);

  return 1;
}

cc_state_t *cc_init(void) {
  cc_state_t *s = malloc(sizeof(cc_state_t));

  s->verbose = 0;
  s->dump_ast= 0;
  s->dump_tokens= 0;
  s->fc= 0;

  return s;
}
