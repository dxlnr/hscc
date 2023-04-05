# Compile tests 
tests:
	ghc main.hs -o hscc
	rm -r hscc.o hscc.hi
	./hscc tests/simple.c
