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

void *hscc_malloc(unsigned long size)
{
    void *ptr;
    ptr = malloc(size);
    if (!ptr && size)
        mem_error("memory full (malloc).");
    return ptr;
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
    struct file_spec *f = hscc_malloc(sizeof *f + strlen(fn));
    f->type = filetype;
    strcpy(f->name, fn);
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
  printf("nb_args: %d\n", nb_args);
  int ca = 1;

  while (ca < nb_args) {
    printf("ca: %d\n", ca);
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
        s->outfile = (*argv)[ca+1];
        ++ca;
      } else {
        return -1;
      }
    }
    ++ca;
    printf("s-> verbose: %d\n", s->verbose);
  
    if (s->verbose) {
      printf("Number of files to compile: %d.", s->fc);
    }
  }

  printf("fc: %d\n", s->fc);
  printf("outfile: %s\n", s->outfile);
  /* printf("verbose: %s\n", s->files); */

  return 1;
}


void write_file_to_buf(cc_state_t *s, const char *fn, int len)
{
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
