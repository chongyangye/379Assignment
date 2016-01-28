/*
CMPUT 379 Assignment 1
Chongyang YE
Student ID:1264608
Unix ID: cye2
*/
#include "memchunk.h"


//This function print out the result we collected from
//memchunk.c.
void print(int size,struct memchunk *chunklist){

    int i;
    for(i=0;i<size;i++){
        if (chunklist[i].RW == 1) {
	        printf ("Chunk # %d is readable and writable from %p\n", i+1,
		      		  chunklist[i].start);
	      	printf ("length: %lu\n", chunklist[i].length);

    
	    }else if (chunklist[i].RW == 0) {
	      	printf ("Chunk # %d is readable only from %p\n", i+1,
		    		  chunklist[i].start);
	      	printf ("length: %lu\n", chunklist[i].length);

    
	    }else{
	        printf ("Chunk # %d is not accessable from %p\n", i+1,
		      		  chunklist[i].start);
	      	printf ("length: %lu\n", chunklist[i].length);

	    } 
    }
    printf ("There is/are %d chunks in total\n", i);     

}

//This is the test file for memchunk.c
int main(){
    int size = 25;
    struct memchunk *chunklist =(struct memchunk *)malloc(100*sizeof(struct memchunk));

    int numChunk = get_mem_layout(chunklist, size);


    if (size<=numChunk){
        print(size,chunklist);
    }else{
        print(numChunk,chunklist);
    }
    free(chunklist);
    return 0;
}
