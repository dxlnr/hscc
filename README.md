# hscc

C Compiler in the making. Basically exploring. Written in C (maybe Haskell someday).

## Overview

```bash
Preprocessor -> Lexing -> Parsing -> Static Analysis -> Intermediate Representation -> Optimization -> Code Generation
```
(1) **Preprocessor**

(2) **Lexing**

The main task of the lexer is to read in the program as a string and convert it to the underlying token of the language. Depending on the compiler design this can be done in one step with parsing.

Using [clang](https://github.com/llvm/llvm-project/tree/main) as a reference, this is what `test/files/simple.c` looks like after the (preprocessing &) lexing step.

```bash
# creating the output via
clang -Xclang -dump-tokens -fsyntax-only test/files/simple.c &> tok_simple_c_clang
# tokens (preprocessor got rid of the references to the headers.)
int             'int'	[StartOfLine]	                Loc=<test/files/simple.c:3:1>
identifier      'main'	              [LeadingSpace]	Loc=<test/files/simple.c:3:5>
l_paren         '('		                                Loc=<test/files/simple.c:3:9>
r_paren         ')'		                                Loc=<test/files/simple.c:3:10>
l_brace         '{'	                  [LeadingSpace]	Loc=<test/files/simple.c:3:12>
int             'int'	[StartOfLine] [LeadingSpace]    Loc=<test/files/simple.c:4:5>
...
semi            ';'		                                Loc=<test/files/simple.c:14:13>
r_brace         '}'	    [StartOfLine]	                Loc=<test/files/simple.c:15:1>
eof             ''		                                Loc=<test/files/simple.c:15:2>
```

(3) Parsing


## Prerequisits 

Haskell compiler that is used: [GHC](https://www.haskell.org/ghc/)

```bash 
apt-get update && apt-get install ghc ghc-prof ghc-doc
```

```bash 
ghc main.hs -o hscc && ./hscc 
```

For the **c** version run

```bash 
make main && ./main tests/simple.c
```

### Token Types

```cpp
class Token {
public:
  enum Kind {
    /// Markers.
    eof,
    error,
    /// Token signifying a code completion location.
    code_complete,
    /// Token signifying a code completion location within a string.
    code_complete_string,

    /// Keywords.
    KW_BEGIN,
    /// Dependent keywords, i.e. those that are treated as keywords depending on
    /// the current parser context.
    KW_DEPENDENT_BEGIN,
    kw_attr,
    kw_op,
    kw_type,
    KW_DEPENDENT_END,

    /// General keywords.
    kw_Attr,
    kw_erase,
    kw_let,
    kw_Constraint,
    kw_Op,
    kw_OpName,
    kw_Pattern,
    kw_replace,
    kw_return,
    kw_rewrite,
    kw_Rewrite,
    kw_Type,
    kw_TypeRange,
    kw_Value,
    kw_ValueRange,
    kw_with,
    KW_END,

    /// Punctuation.
    arrow,
    colon,
    comma,
    dot,
    equal,
    equal_arrow,
    semicolon,
    /// Paired punctuation.
    less,
    greater,
    l_brace,
    r_brace,
    l_paren,
    r_paren,
    l_square,
    r_square,
    underscore,

    /// Tokens.
    directive,
    identifier,
    integer,
    string_block,
    string
  };
```

## Additional Information

- [Compilers - Principles, Techniques & Tools](https://en.wikipedia.org/wiki/Compilers:_Principles,_Techniques,_and_Tools)
- [Crafting Interpreters](https://craftinginterpreters.com/) by Robert Nystrom is the source.
- [pycparser](https://github.com/eliben/pycparser): Complete C99 parser in pure Python
