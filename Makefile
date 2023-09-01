CC=		gcc
CFLAGS=		-g -Wall -std=gnu99 -Iinclude

bminor: encoder/string.o bminor.o
	$(CC) $(CFLAGS) -o $@ $^

encoder/string.o: encoder/string.c encoder/string.h
	$(CC) $(CFLAGS) -c -o $@ $<

bminor.o: bminor.c encoder/string.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm *.o 
	rm -f encoder/*.o
	rm test/encode/*.out
	rm bminor

all: bminor

test: bminor encode_test

encode_test: runtest.sh
	./runtest.sh
	