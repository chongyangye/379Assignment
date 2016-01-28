//get from ip Packet use routing table come out statistic file

//need id compare the id address with bits
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFLEN 512
#define PORT 9930
#define IP 2130706433  /* 127.0.0.1 */

int flag=1;


void sig_hander(int signo){
	flag =0;
}
char *decimal_to_binary(int n)
{
   int c, d, count;
   char *pointer;
 
   count = 0;
   pointer = (char*)malloc(32+1);
 
   for ( c = 31 ; c >= 0 ; c-- )
   {
         d = n >> c;
    
         if ( d & 1 )
            *(pointer+count) = 1 + '0';
         else
            *(pointer+count) = 0 + '0';
    
         count++;
      }
   *(pointer+count) = '\0';
 
   return  pointer;
}

in_addr_t to_mask(int n){
	int r = 0;
	for(int i = 0; i< n; i++){
		r = r | (1 << i);
	} 

	r = r << (32-n);

	return (in_addr_t) r;

}




int main(int argc,  char *argv[]){
	
	FILE *table=fopen("RT_A.txt","r");	
	FILE *result=fopen("result.txt","w+");
	struct sockaddr_in si_me, si_other;
	int s, slen=sizeof(si_other);
	char buf[BUFLEN];

	



	
/***                                                   ***/

	int  TTL, p_id, Bita=0,Bitb=0,Bitc=0,num=0;

	char from[20], To[20], payload[20], A[20],B[20],C[20],Ra[10],Rb[10],Rc[10];
	char *get_from, *get_To, *get_payload,line[10][100];



/***                                                  ***/
	if ( ( s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) ) == -1 )
	{
		printf("Error in creating socket");
		return 1;
	}

	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(IP); /* htonl(INADDR_ANY) for any interface on this machine */

	if ( bind(s, &si_me, sizeof(si_me)) == -1 )
	{
		printf("Error in binding the socket");
		return 2;
	}

/*****        get the basic info from R_T            ****/

	while ( fgets ( line[num],100, table ) != NULL ) /* read a line */
	{
		num++;
	}
	printf("%s\n",line[0]);
	printf("%s\n",line[1]);
	printf("%s\n",line[2]);
	printf("%s\n",line[3]);
	printf("%s\n",line[4]);
	for (int j = 0; j<=num;j++){
		if (strlen(line[j])>1){
			if (Bitb==0)
				sscanf(line[j],"%s %d %s",B,&Bitb,Rb);
			else if(Bita==0)
				sscanf(line[j],"%s %d %s",A,&Bita,Ra);
			else if(Bitc==0)
				sscanf(line[j],"%s %d %s",C,&Bitc,Rc);
		}

	}

	printf("%s,%d,%s,%s,%d,%s,%s,%d,%s\n",A,Bita,Ra,B,Bitb,Rb,C,Bitc,Rc);


/*****    for each address in A,B,C split in to 4 small one             ****/
	printf("A:%s,  B:%s,  C:%s\n",A,B,C);
	 
	in_addr_t addra = inet_network(A), addrb= inet_network(B), addrc= inet_network(C);
	in_addr_t maska = to_mask(Bita),maskb = to_mask(Bitb),maskc = to_mask(Bitc);


	printf("\n\nServer listening to %s:%d\n\n", inet_ntoa(si_me.sin_addr), ntohs(si_me.sin_port));
	int To_A=0,To_B=0,To_C=0,To_I=0, expir = 0;


	(void)signal(SIGINT,sig_hander);




	while (flag) {
		int flaga = 0,flagb = 0, flagc = 0, flagm = 0, flagflag = 0;
		if ( recvfrom(s, buf, BUFLEN, 0, &si_other, &slen) != -1)
			{
			
			printf("\nReceived packet from %s:%d  Data: %s\n\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
/***                                 from the data from the buffer                          ***/
			sscanf(buf,"<%d> %s %s <%d> %s\n",&p_id,from,To,&TTL,payload);

			get_from=from;			
			get_from++;
			get_from[strlen(get_from)-1]=0;

			get_To=To;			
			get_To++;
			get_To[strlen(get_To)-1]=0;

			get_payload=payload;			
			get_payload++;
			get_payload[strlen(get_payload)-1]=0;
/****         get_from, get_To is the two string need to compare       ***/           
			
			in_addr_t addrFrom = inet_network(get_from), addrTo=inet_network(get_To);
	
/***	                start to compare                      ***/

		
			if (TTL>1){
				printf("To B?::%d, %s\n",addrFrom,decimal_to_binary(addrFrom));
				flagm = (int)maskb & (int)addrTo;
				flagb = (int)addrb ^ flagm;
				//printf("To B?::%d, %s\n",flagb,decimal_to_binary(flagb));
				if (flagb ==0){
					To_B++;
					flagflag =1;
					}
				flagm = (int)maska & (int)addrTo;
				flaga = (int)addra ^ (int)flagm;
				//printf("To A?::%d, %s\n",flaga,decimal_to_binary(flaga));
				if (flaga ==0){
					To_A++;
					flagflag = 1;
			
				}
				flagm = (int)maskc & (int)addrTo;
				flagc = (int)addrc ^ (int)flagm;
				//printf("To C?::%d, %s\n",flagc,decimal_to_binary(flagc));
				if (flagc ==0){
					To_C++;
					flagflag = 1;
				}
				if (flagflag == 1)
					To_I++;						
			}
			else{
				//printf("expired %d\n",expir);
				expir++;
			}
			if (p_id%20==0){
			
				fseek(result,0,SEEK_SET);
				fprintf(result,"expired packets: %d\n",expir);
				fprintf(result,"unroutable packets: %d\n",To_I);
				fprintf(result,"delivered direct: %d\n",To_A);
				fprintf(result,"router B: %d\n",To_B);
				fprintf(result,"router C: %d\n",To_C);

			
			}


/***                ***/
		}

	}
	fseek(result,0,SEEK_SET);
	fprintf(result,"expired packets: %d\n",expir);
	fprintf(result,"unroutable packets: %d\n",To_I);
	fprintf(result,"delivered direct: %d\n",To_A);
	fprintf(result,"router B: %d\n",To_B);
	fprintf(result,"router C: %d\n",To_C);


	fclose(result);
	fclose(table);
	return 0;

	

	
}
