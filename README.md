# hscc

Little C Compiler in the making. Basically exploring. Written in C (maybe Haskell someday).

## Overview

```bash
Preprocessor -> Lexing -> Parsing -> Static Analysis -> Intermediate Representation -> Optimization -> Code Generation
```
(1) **Preprocessor**

The preprocessor is a tool that processes the source code before it is passed to the actual compiler. The primary tasks are:

- **Removing Comments**: The preprocessor strips out comments from the source code.
- **Macro Expansion**: Replace macros with their definitions. Macros are typically defined using the `#define` directive.
- **File Inclusion**: Include the contents of one file into another.
- **Conditional Compilation**: Including or excluding parts of the code based on certain conditions using directives like `#if`, `#ifdef`, `#ifndef`, `#else`, `#elif`, and `#endif`.

Example using clang (clang -E)
```bash
extern int fputs (const char *__restrict __s, FILE *__restrict __stream);

extern int ungetc (int __c, FILE *__stream);
...
# 885 "/usr/include/stdio.h" 3 4
extern int __uflow (FILE *);
extern int __overflow (FILE *, int);
# 2 "test/files/simple.c" 2

int main() {
    int num;
```

(2) **Lexing**

The main task of the lexer is to read in the program as a string and convert it to the underlying token of the language. Depending on the compiler design this can be done in one step with parsing.

Using [clang](https://github.com/llvm/llvm-project/tree/main) as a reference, this is what `test/files/simple.c` looks like after the (preprocessing &) lexing step.

```bash
# creating the output via
clang -Xclang -dump-tokens -fsyntax-only test/files/simple.c &> tok_simple_c_clang
# tokens (preprocessor got rid of the references to the headers.)
int             'int'   [StartOfLine]	                Loc=<test/files/simple.c:3:1>
identifier      'main'                  [LeadingSpace]	Loc=<test/files/simple.c:3:5>
l_paren         '('                                     Loc=<test/files/simple.c:3:9>
r_paren         ')'                                     Loc=<test/files/simple.c:3:10>
l_brace         '{'                     [LeadingSpace]	Loc=<test/files/simple.c:3:12>
int             'int'   [StartOfLine]   [LeadingSpace]  Loc=<test/files/simple.c:4:5>
...
semi            ';'                                     Loc=<test/files/simple.c:14:13>
r_brace         '}'     [StartOfLine]	                Loc=<test/files/simple.c:15:1>
eof             ''                                      Loc=<test/files/simple.c:15:2>
```

(3) **Parsing**


## Run the Compiler

For the **c** version, run the bash script and provide an input file
```bash 
./run.sh test/files/simple.c
```

## Additional Information

- [Compilers - Principles, Techniques & Tools](https://en.wikipedia.org/wiki/Compilers:_Principles,_Techniques,_and_Tools): Textbook about compiler construction for programming languages.
- [Crafting Interpreters](https://craftinginterpreters.com/) by Robert Nystrom is a great source.
- [TinyCC](https://github.com/TinyCC/tinycc): The Smallest ANSI C compiler.
- [pycparser](https://github.com/eliben/pycparser): Complete C99 parser in pure Python.
