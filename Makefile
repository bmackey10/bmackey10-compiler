CC=			gcc
CFLAGS=		-g -Wall -std=gnu99 -Iinclude
LEX= 		flex

bminor: encoder/encode.o scanner/scan.o bminor.o
	$(CC) $(CFLAGS) -o $@ $^

scanner/scan.o: scanner/scan.c scanner/scan.h
	$(CC) $(CFLAGS) -c -o $@ $<

scanner/scan.c: scanner/scan.flex scanner/scan.h
	$(LEX) -o $@ $<

encoder/encode.o: encoder/encode.c encoder/encode.h
	$(CC) $(CFLAGS) -c -o $@ $<

bminor.o: bminor.c encoder/encode.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o 
	rm -f encoder/*.o
	rm -f test/encode/*.out
	rm -f test/scan/*.out
	rm -f test/encode/hidden/*.out
	rm -f test/scan/book/*.out
	rm -f scanner/*.o
	rm -f scanner/*.c
	rm -f bminor

all: bminor

test: bminor encode_test

encode_test: runtest.sh
	./runtest.sh
	