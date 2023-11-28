GCC=		gcc
CC=			cc
CFLAGS=		-g -Wall -std=gnu99 -Iinclude -lm
LEX= 		flex
BISON=		bison
BFLAGS=		--defines=parser/token.h --output=parser/parse.c -v

bminor: bminor.o parser/parse.o encoder/encode.o scanner/scan.o resolver/resolve.o decl.o expr.o param_list.o type.o symbol.o stmt.o hash_table.o scope.o typecheck.o
	$(GCC) $(CFLAGS) -o $@ $^

bminor.o: bminor.c
	$(CC) $(CFLAGS) -c -o $@ $<

parser/parse.o: parser/parse.c
	$(CC) $(CFLAGS) -c -o $@ $<

parser/parse.c: parser/parse.bison
	$(BISON) $(BFLAGS) $<

encoder/encode.o: encoder/encode.c encoder/encode.h parser/parse.o
	$(CC) $(CFLAGS) -c -o $@ $< 

scanner/scan.o: scanner/scan.c
	$(CC) $(CFLAGS) -c -o $@ $<

scanner/scan.c: scanner/scan.flex
	$(LEX) -o $@ $<

resolver/resolve.o: resolver/resolve.c resolver/resolve.h
	$(CC) $(CFLAGS) -c -o $@ $<

decl.o: decl.c decl.h
	$(CC) $(CFLAGS) -c -o $@ $<

expr.o: expr.c expr.h
	$(CC) $(CFLAGS) -c -o $@ $<

param_list.o: param_list.c
	$(CC) $(CFLAGS) -c -o $@ $<

type.o: type.c type.h
	$(CC) $(CFLAGS) -c -o $@ $<

symbol.o: symbol.c symbol.h
	$(CC) $(CFLAGS) -c -o $@ $<

stmt.o: stmt.c stmt.h decl.h
	$(CC) $(CFLAGS) -c -o $@ $<

hash_table.o: hash_table.c hash_table.h
	$(CC) $(CFLAGS) -c -o $@ $<

scope.o: scope.c scope.h
	$(CC) $(CFLAGS) -c -o $@ $<

typecheck.o: typecheck.c typecheck.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o 
	rm -f encoder/*.o
	rm -f scanner/*.o
	rm -f scanner/*.c
	rm -f parser/*.o
	rm -f parser/*.c
	rm -f parser/*.output
	rm -f resolver/*.o
	rm -f test/encode/*.out
	rm -f test/scan/*.out
	rm -f test/parse/*.out
	rm -f test/print/*.out
	rm -f test/resolve/*.out
	rm -f test/typecheck/*.out
	rm -f test/encode/hidden/*.out
	rm -f test/scan/book/*.out
	rm -f test/parse/hidden/*.out
	rm -f test/print/hidden/*.out
	rm -f test/resolve/hidden/*.out
	rm -f test/typecheck/hidden/*.out
	rm -f bminor

all: bminor

test: bminor encode_test

encode_test: runtest.sh
	./runtest.sh
	