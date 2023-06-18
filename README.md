# hscc

C Compiler 

First task is to implement a lexer using a finite state machine.


## Prerequisits 

Haskell compiler that is used: [GHC](https://www.haskell.org/ghc/)

```bash 
apt-get update && apt-get install ghc ghc-prof ghc-doc
```


## Run 

```bash 
ghc main.hs -o hscc && ./hscc 
```


## Additional Information

- [Compilers - Principles, Techniques & Tools](https://en.wikipedia.org/wiki/Compilers:_Principles,_Techniques,_and_Tools)
- [Crafting Interpreters](https://craftinginterpreters.com/) by Robert Nystrom is the source.
- https://norasandler.com/2017/11/29/Write-a-Compiler.html
