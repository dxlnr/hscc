# hscc

C Compiler 

## Overview

```bash
Lexing -> Parsing -> Static Analysis -> Intermediate Representation -> Optimization -> Code Generation
```

## Prerequisits 

Haskell compiler that is used: [GHC](https://www.haskell.org/ghc/)

```bash 
apt-get update && apt-get install ghc ghc-prof ghc-doc
```

## Run 

```bash 
ghc main.hs -o hscc && ./hscc 
```

For the **c** version run
```bash 
make main && ./main tests/simple.c
```

### 
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
- https://norasandler.com/2017/11/29/Write-a-Compiler.html
