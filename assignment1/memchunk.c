/*
CMPUT 379 Assignment 1
Chongyang YE
Student ID:1264608
Unix ID: cye2
*/

#include "memchunk.h"

static sigjmp_buf sigbuf;
static struct sigaction newsa, oldsa;

int status = 0;
int preStatus=0;
unsigned long currentAddress;

//set jump, jump to while loop
void handler(int sig){
    siglongjmp(sigbuf,1);
}
//initial the handler
void initialHandler(){
	newsa.sa_handler = handler;
	sigemptyset(&newsa.sa_mask);
	newsa.sa_flags = 0;
	if (sigaction(SIGSEGV, NULL, &oldsa) == -1)
		err(1, "can't save old sigaction");
	if (sigaction(SIGSEGV, &newsa, NULL) == -1)
		err(1, "can't do new sigaction");

}
//scan the memory and return the number of actual number of memory
//chunks located in the process's address space

int get_mem_layout (struct memchunk *chunk_list, int size)
{
    unsigned long endPage = 0xffffffff;
    unsigned int pageNum = endPage/PAGE_SIZE;
    unsigned long startPage = 0x00000000;
    int numberOfChunk=0;
    char str = 'a';
    int firstPage = 1;
    currentAddress = startPage;

    
//initial the handler   
	initialHandler();
	

    int page = 0;
    while(page<pageNum){
        page++;
        sigsetjmp(sigbuf,1);
        //check if the page of memory is accessable
        if(status ==0){
        
            if(firstPage ==1){
                firstPage =0;
                chunk_list[numberOfChunk].start = (void *) currentAddress;
				chunk_list[numberOfChunk].length = PAGE_SIZE; 
				chunk_list[numberOfChunk].RW=status-1;
				preStatus =status;
                status=0;
                currentAddress += PAGE_SIZE;
            }else{
                //check if the last page is not accessable
                if(preStatus ==0){
                    chunk_list[numberOfChunk].length+=PAGE_SIZE;
                    currentAddress += PAGE_SIZE;
                    preStatus=status;	
			        status=0;
                }else{
                    numberOfChunk+=1;
                    chunk_list[numberOfChunk].start = (void *) currentAddress;
				    chunk_list[numberOfChunk].length = PAGE_SIZE; 
				    chunk_list[numberOfChunk].RW=status-1;
				    currentAddress += PAGE_SIZE;
				    preStatus =status;
                    status=0;
                }
            }//check if the last page has different accessability with this one
        }else if((numberOfChunk<size) && (preStatus != status)&&(status!=0)){
            if(firstPage==1){
                firstPage = 0;
                chunk_list[numberOfChunk].start = (void *) currentAddress;
				chunk_list[numberOfChunk].length = PAGE_SIZE; 
				chunk_list[numberOfChunk].RW=status-1;
				preStatus =status;
                status=0;

				    	
            }else{
                numberOfChunk+=1;
                chunk_list[numberOfChunk].start = (void *) currentAddress;
				chunk_list[numberOfChunk].length = PAGE_SIZE; 
				chunk_list[numberOfChunk].RW=status-1;
				preStatus =status;
                status=0;

                
            }
            currentAddress += PAGE_SIZE;
        }else if((status!=0)&&(status == preStatus)){
            chunk_list[numberOfChunk].length += PAGE_SIZE;
            currentAddress += PAGE_SIZE;
            preStatus=status;	
			status=0;
			
        }
        
        
        //make sure if this is the end of page and return value
        if (currentAddress==(endPage-PAGE_SIZE+1) || numberOfChunk==size-1)
		{
			return numberOfChunk+1;
		}

		status=0;
		// check if it's readable   
		str = *((char *) currentAddress);
		status = 1;

		// check if it's writeable
		*((char *) currentAddress) = str;
		status = 2;
        
        
        

        
        
    
    
    }
    return numberOfChunk+1;

}
