
all: file_server file_client

file_server: file_server.o
	gcc -Wall -o file_server file_server.o -lm 

file_server.o: file_server.c 
	gcc -Wall -c file_server.c 

file_client: file_client.o
	gcc -Wall -o file_client file_client.o -lm

file_client.o: file_client.c 
	gcc -Wall -c file_client.c


clean:
	rm -f *.o file_server file_client
