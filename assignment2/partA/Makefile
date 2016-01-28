
all: router pktgen

router: router.o
	gcc -Wall -o router router.o -lm

router.o: router.c 
	gcc -Wall -c router.c

pktgen: pktgen.o
	gcc -Wall -o pktgen pktgen.o -lm

pktgen.o: pktgen.c 
	gcc -Wall -c pktgen.c
	
clean:
	-rm -f *.o router pktgen core
