memtest: memchunk.o test.o
	gcc memchunk.o test.o -o memtest -m32	

memchunk.o: memchunk.c memchunk.h
	gcc -c -Wall memchunk.c -m32

test.o: test.c
	gcc -c -Wall test.c -m32

clean:
	rm -f *.o memtest
