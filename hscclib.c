/*
 *  HSCC C Compiler Library Implementation.
 * 
*/
#include "hscc.h"

int strin(char **arr, int len, char *s) {
  for(int i = 0; i < len; ++i) {
    if(strncmp(arr[i], s, strlen(s)) == 0) {
      return 1;
    }
  }
  return 0;
}

void cc_free(void *ptr) {
  free(ptr);
}

void cc_close_buf(file_buffer_t *file)
{
    file_buffer_t *bf = file;
    if (bf->fd > 0) {
        close(bf->fd);
    }
    file = bf->prev;
    cc_free(bf);
}

void tcc_memcheck(int flag)
{
}

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

static void dynarray_reset(void *pp, int *n)
{
  void **p;
  for (p = *(void***)pp; *n; ++p, --*n)
    if (*p)
      cc_free(*p);
  cc_free(*(void**)pp);
  *(void**)pp = NULL;
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
  arg_option_ignored      = 0,
  arg_option_help         = 1,
  arg_option_verbose      = 2,
  arg_option_o            = 3,
  arg_option_dump_ast     = 4,
  arg_option_dump_tokens  = 5,
};

static const arg_options_t args_options [] = {
  { "h",            arg_option_help,        0 },
  { "-verbose",     arg_option_verbose,     0 },
  { "-dump-ast",    arg_option_dump_ast,    0 },
  { "-dump-tokens", arg_option_dump_tokens, 0 },
  { "-o",           arg_option_o,           0 },
  { NULL,           arg_option_ignored,     0 }
};

int parse_args(cc_state_t *s, int *argc, char ***argv) 
{
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
    printf("(v) Files specified for compilation: %d\n", s->fc);
  }
  return 0;
}

char *cc_get_file_ext(const char *fn) 
{
  for (int i = strlen(fn) - 1; i >= 0; --i) {
    if (fn[i] == '.') {
      return (char *)fn + i;
    }
  }
  return NULL;
}

int write_file_to_buf(cc_state_t *s, const char *fn)
{
  int fd = open(fn, O_RDONLY);
  if (fd < 0) {
    printf("Error: %s\n", "File not found.");
    return -1;
  }

  struct stat st;
  if (stat(fn, &st) != 0) {
    perror("Error: Failed to get file size.");
    close(fd);
    return -1;
  }
  size_t fsize = st.st_size;

  file_buffer_t *buf = (file_buffer_t *)malloc(sizeof(file_buffer_t) + fsize - 1);
  if (!buf) {
    perror("Error: Failed to allocate enough memory.");
    close(fd);
    return -1;
  }

  buf->fd = fd;
  buf->line_num = 1;
  memcpy(buf->fn, fn, sizeof(buf->fn));
  buf->buf_end = buf->buf + fsize;
  buf->buf_ptr = buf->buf;

  ssize_t bytesRead = read(fd, buf->buf, fsize);
  if (bytesRead != fsize) {
    perror("Failed to read the entire file.");
    free(buf);
    close(fd);
    return -1;
  }
  s->fb = buf;

  return fd;
}

int cc_add_file(cc_state_t *s, const char *fn) 
{
  char *ext = cc_get_file_ext(fn);
  if (ext == NULL) {
    printf("Error: %s\n", "No file extension.");
    return -1;
  }
  if (!strcmp(ext, ".c") || !strcmp(ext, ".h")) {
    s->filetype = FILE_TYPE_C;
  } else if (!strcmp(ext, ".s") || !strcmp(ext, ".S")) {
    s->filetype = FILE_TYPE_ASM;
  } else {
    printf("Error: %s\n", "Unknown file extension.");
    return -1;
  }
  return write_file_to_buf(s, fn);
}

int cc_compile(cc_state_t *s, const char *str, int fd) {
  int ret;

  if (setjmp(s->err_jmp_buf) == 0) {
    s->nb_errors = 0;

    ret = cc_preprocess(s);
  }
  return s->nb_errors != 0 ? -1 : 0;
}

cc_state_t *cc_init(void) {
  cc_state_t *s = malloc(sizeof(cc_state_t));
  if (!s) return NULL;

  s->files = NULL;
  s->verbose = 0;
  s->dump_ast= 0;
  s->dump_tokens= 0;
  s->fc= 0;
  s->outfile = NULL;

  return s;
}

void cc_delete(cc_state_t *s) 
{
  dynarray_reset(&s->files, &s->fc);
  free(s);

#ifdef MEM_DEBUG
  cc_memcheck(-1);
#endif
}
