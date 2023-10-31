GCC=		gcc
CC=			cc
CFLAGS=		-g -Wall -std=gnu99 -Iinclude -lm
LEX= 		flex
BISON=		bison
BFLAGS=		--defines=parser/token.h --output=parser/parse.c -v

bminor: parser/parse.o scanner/scan.o encoder/encode.o bminor.o decl.o expr.o param_list.o type.o symbol.o stmt.o
	$(GCC) $(CFLAGS) -o $@ $^

bminor.o: bminor.c encoder/encode.h
	$(CC) $(CFLAGS) -c -o $@ $<

encoder/encode.o: encoder/encode.c encoder/encode.h
	$(CC) $(CFLAGS) -c -o $@ $<

scanner/scan.o: scanner/scan.c parser/token.h
	$(CC) $(CFLAGS) -c -o $@ $<

scanner/scan.c: scanner/scan.flex parser/token.h
	$(LEX) -o $@ $<

parser/parse.o: parser/parse.c decl.o expr.o param_list.o type.o symbol.o stmt.o
	$(CC) $(CFLAGS) -c -o $@ $<

parser/parse.c: parser/parse.bison decl.o expr.o param_list.o type.o symbol.o stmt.o
	$(BISON) $(BFLAGS) $<

decl.o: decl.c decl.h type.o stmt.o expr.o
	$(CC) $(CFLAGS) -c -o $@ $<

expr.o: expr.c expr.h
	$(CC) $(CFLAGS) -c -o $@ $<

param_list.o: param_list.c param_list.h
	$(CC) $(CFLAGS) -c -o $@ $<

type.o: type.c type.h
	$(CC) $(CFLAGS) -c -o $@ $<

symbol.o: symbol.c symbol.h
	$(CC) $(CFLAGS) -c -o $@ $<

stmt.o: stmt.c stmt.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o 
	rm -f encoder/*.o
	rm -f scanner/*.o
	rm -f scanner/*.c
	rm -f parser/*.o
	rm -f parser/*.c
	rm -f parser/*.output
	rm -f parser/token.h
	rm -f test/encode/*.out
	rm -f test/scan/*.out
	rm -f test/parse/*.out
	rm -f test/encode/hidden/*.out
	rm -f test/scan/book/*.out
	rm -f test/parse/hidden/*.out
	rm -f bminor

all: bminor

test: bminor encode_test

encode_test: runtest.sh
	./runtest.sh
	