/*
CMPUT 379 Assignment 3
Chongyang YE
Student ID:1264608
Unix ID: cye2
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>


int flag = 1;
void sig_handler(int sig)
{
	flag = 0;
}


int main(int argc, char * argv[]){
    time_t t;
    //collect input
    int resourceType,process;
    
    printf("Number of different resource types: ");
    scanf("%d",&resourceType);
    
    int MaximumAvaRes[resourceType],i,j;
    printf("Number of instances of each resource types: ");
    for(i=0;i<resourceType;i++){
        scanf("%d",&MaximumAvaRes[i]);
        
    }
    printf("Number of process: ");
    scanf("%d",&process);
    int alloc[process][resourceType];
    int currAva[resourceType],request[process][resourceType],need[process][resourceType];
    srand((unsigned) time(&t));
    //generate allocation
    int useAva[resourceType];
    int possible[process][resourceType];
    for(i=0;i<process;i++){
        printf("P%d: ",i+1);
        for(j=0;j<resourceType;j++){
            scanf("%d",&possible[i][j]);

       
        }

    
    }
    printf("1");
    for(i=0;i<process;i++){
        printf("P%d: ",i+1);
        for(j=0;j<resourceType;j++){
            if(possible[i][j]<=0){
                alloc[i][j]=0;
            }else{
                alloc[i][j]=rand()%possible[i][j];
            }
        }
    
    }
    

    for(i=0;i<resourceType;i++){
        int temp=0;
        for(j=0;j<process;j++){
            temp=temp+alloc[j][i];
        }
        useAva[i]=MaximumAvaRes[i]-temp;
    }

    (void) signal(SIGINT, sig_handler);
    int counter =0;
    while(flag){
        //print the information
        
        for(i=0;i<process;i++){
            for(j=0;j<resourceType;j++){
                need[i][j]=possible[i][j]-alloc[i][j];
            }
        }
        srand((unsigned) time(&t));
        for(i=0;i<process;i++){
            for(j=0;j<resourceType;j++){
                if(possible[i][j]-alloc[i][j]<=0){
                    request[i][j]=0;
                }else{
                
                    request[i][j]=rand()%(possible[i][j]-alloc[i][j]);
                }
            }
        }
        for(i=0;i<process;i++){
            for(j=0;j<resourceType;j++){
                printf("%d ",request[i][j]);
            }
            printf("\n");
        }
        printf("\nCurrent Allocation\n");
        for(i=0;i<process;i++){
            printf("%d: ", i+1);
            for(j=0;j<resourceType;j++){
                printf("%d ",alloc[i][j]);
            }
            printf("\n");
        }
        
        printf("\nCurrent Request\n");
        for(i=0;i<process;i++){
            printf("%d: ", i+1);
            for(j=0;j<resourceType;j++){
                printf("%d ",request[i][j]);
            }
            printf("\n");
        } 
        
        printf("\nCurrent Available Resources\n");
        for(i=0;i<resourceType;i++){
                printf("%d ",currAva[i]);
          

        }
        
        srand((unsigned) time(&t));
        for(j=0;j<resourceType;j++){
            currAva[j]=useAva[j];
         }
         
        
        printf("\nMaximum Possible Request\n");
        for(i=0;i<process;i++){
            printf("%d: ", i+1);
            for(j=0;j<resourceType;j++){
                printf("%d ",possible[i][j]);
            }
            printf("\n");
        } 
        
        printf("\nMax Available Resource\n");
        for(i=0;i<resourceType;i++){

                printf("%d ",MaximumAvaRes[i]);
          

        }
        printf("\n");
        int checkAvaliable =0,n,checkNeed=0;
        //compare the request, need with current avaliable
        for(i=0;i<process;i++){

            for(j=0;j<resourceType;j++){
                if(request[i][j]>currAva[j]){
                    checkAvaliable =1;
                }
                
                if(need[i][j]>currAva[j]){
                    checkNeed =1;
                }
               
            }
            
            //if not satisfied, just exit   
            if (checkAvaliable ==1 || checkNeed ==1){
                printf("Request ");
                for(n=0;n<resourceType;n++){
                    printf("%d ",request[i][n]);
                }
                printf(" from P%d cannot be satisfied, P%d is in waiting state\n", i+1,i+1);
                exit(1);
                
            }
            printf("Request ");
            for(n=0;n<resourceType;n++){
                printf("%d ",request[i][n]);
            }
            printf("from P%d has been granted\n", i+1);
            for(n=0;n<resourceType;n++){
                currAva[n]=currAva[n]+request[i][n];
            }
            printf(" P%d has released ", i+1);
            for(n=0;n<resourceType;n++){
                printf("%d ",request[i][n]);
            }
            printf("\n");
        }
        if(checkAvaliable ==1 ||checkNeed ==1){
            break;
        }
        
        
        
        
        
    
        printf("\n===============================\n");
        if(counter%5==0){
            sleep(2);
        }
        counter++;
    }
   
    printf("Simulation has been ended.\n");
    return 0;
}
