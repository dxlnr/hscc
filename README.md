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

Example using clang (`clang -E`)

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
...
```

An algorithmic outline can be found [here](resources/cpp.algo.pdf). 

For an additional read: [Under the Hood: warp, a fast C and C++ preprocessor](https://engineering.fb.com/2014/03/28/open-source/under-the-hood-warp-a-fast-c-and-c-preprocessor/), which was project done at Meta and is open-source.

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

The main goal for the parser is to transform the token stream into a logical and contextual structure.
This structure is called abstract syntax tree (or an AST) which is a tree-shaped representation of source code.

```bash
# dump the ast to console.
clang -Xclang -dump-tokens test/files/simple.c
# AST (partly)
`-FunctionDecl 0xe60860 <test/files/simple.c:3:1, line:20:1> line:3:5 main 'int ()'
  `-CompoundStmt 0xe61d88 <col:12, line:20:1>
    |-DeclStmt 0xe60980 <line:4:5, col:12>
    | `-VarDecl 0xe60918 <col:5, col:9> col:9 used num 'int'
    |-CallExpr 0xe60a80 <line:5:5, col:32> 'int'
    | |-ImplicitCastExpr 0xe60a68 <col:5> 'int (*)(const char *, ...)' <FunctionToPointerDecay>
    | | `-DeclRefExpr 0xe60998 <col:5> 'int (const char *, ...)' Function 0xe45898 'printf' 'int (const char *, ...)'
    | `-ImplicitCastExpr 0xe60ac0 <col:12> 'const char *' <NoOp>
    |   `-ImplicitCastExpr 0xe60aa8 <col:12> 'char *' <ArrayToPointerDecay>
    |     `-StringLiteral 0xe609f8 <col:12> 'char[19]' lvalue "Enter an integer: "

```


## Run the Compiler

For the **c** version, run the bash script and provide an input file
```bash 
# Checkout out what it can do for you.
./run.sh
# Compile
./run.sh test/files/simple.c -verbose
# Output tokens only 
./run.sh test/files/simple.c -dump-tokens
```

## Additional Information

- [Compilers - Principles, Techniques & Tools](https://en.wikipedia.org/wiki/Compilers:_Principles,_Techniques,_and_Tools): Textbook about compiler construction for programming languages.
- [Crafting Interpreters](https://craftinginterpreters.com/) by Robert Nystrom is a great source.
- [TinyCC](https://github.com/TinyCC/tinycc): The Smallest ANSI C compiler.
- [pycparser](https://github.com/eliben/pycparser): Complete C99 parser in pure Python.
- [Compiler Programming](https://www.youtube.com/watch?v=wWTon5J2uhs&list=PLmV5I2fxaiCIZVTLzofsocka2LvWBFvBa) feat. Jonathan Blow. Just extensive
- [tiny-c](http://www.iro.umontreal.ca/~felipe/IFT2030-Automne2002/Complements/tinyc.c) by Marc Feely.
