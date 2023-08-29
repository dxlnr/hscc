/*
 *  HSCC C Compiler Library Implementation.
 * 
*/
#include "hscc.h"

void mem_error(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit (1);
}

static void dynarray(void* ptab, int *sptr, void *data)
{
  int nb = *sptr;
  int nb_alloc;

  void **pp;
  pp = *(void ***)ptab;

  if ((nb & (nb - 1)) == 0) {
    if (!nb) {
      nb_alloc = 1;
    } else {
      nb_alloc = nb * 2;
    }
    pp = realloc(pp, nb_alloc * sizeof(void *));
    *(void***) ptab = pp;
  }
  pp[nb++] = data;
  *sptr = nb;
}

static void args_parser_add_file(cc_state_t *s, const char* fn, int filetype)
{
  struct file_spec *f = malloc(sizeof *f + strlen(fn));
  f->type = filetype;
  strcpy(f->name, fn);
  dynarray(&s->files, &s->fc, f);
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

char *read_from_file(char *fname) 
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

  if (nb_args < 2) {
    return 1;
  }
  while (ca < nb_args) {
    char *arg = (*argv)[ca];

    if (*arg != '-' && *arg != '\0') {
      args_parser_add_file(s, arg, s->filetype);
    } else {
      if (strcmp(arg, "-verbose") == 0) {
        s->verbose = 1;
      } else if (strcmp(arg, "-dump-ast") == 0) {
        s->dump_ast = 1;
      } else if (strcmp(arg, "-dump-tokens") == 0) {
        s->dump_tokens = 1;
      } else if (strcmp(arg, "-o") == 0) {
        if (ca + 1 >= nb_args) {
          return -1;
        }
        ca++;
        s->outfile = (*argv)[ca++];
      } else {
        return -1;
      }
    }
    ++ca;
  }
  if (s->verbose) {
    printf("files to compile: %d \n", s->fc);
  }
  return 0;
}


void write_file_to_buf(cc_state_t *s, const char *fn, int len)
{
}


int cc_run_file(cc_state_t *s, const char *fn) {
  return 0;
}

cc_state_t *cc_init(void) {
  cc_state_t *s = malloc(sizeof(cc_state_t));

  s->files = NULL;
  s->verbose = 0;
  s->dump_ast= 0;
  s->dump_tokens= 0;
  s->fc= 0;
  s->outfile = NULL;

  return s;
}

void cc_delete(cc_state_t *s) {
  free(s);
}

void hscc_free(void *ptr)
{
    free(ptr);
}
