#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <setjmp.h>
#include <time.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>

#define IP 2130706433  /* 127.0.0.1 */
#define SERVERPORT 9930
#define PORT 9931

int flag=1;


void sig_hander(int signo){
	flag =0;
}


int main(int argc,  char *argv[]){

/***               ***/
	int from,To,TTL,p_id;
	int AtoB = 0,AtoC = 0,BtoA = 0,BtoC = 0,CtoA = 0,CtoB = 0,InvD = 0;
	time_t t;
	FILE *f = fopen("file.txt", "w+");
	srand((unsigned)time(&t));
	char buffer[100];
	char *payload = "Hello:)";
	const char *addr_ip[10]={"192.168.192.4","192.168.192.6","192.168.192.10",
			"192.168.128.7","192.168.128.1","192.224.0.5","192.224.10.5",
			"192.224.0.7","192.224.15.6","168.130.192.01"};

/***                              ***/

	struct sockaddr_in si_me, si_other;
	int s, i, slen=sizeof(si_other);


/****
	if ( argc != 2 )
	{
		printf("\n\nusage: %s <int value>\n\n", argv[0]);
		return 3;
	}

	//int port = argv[1];***/

	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(IP);    /* htonl(INADDR_ANY) for any interface on this machine */

	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(SERVERPORT);
	si_other.sin_addr.s_addr = htonl(IP);

	if ( ( s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) ) == -1 )
	{
		printf("Error in creating socket");
		return 1;
	}



	if ( bind(s, &si_me, sizeof(si_me)) == -1 )
	{
		printf("Error in binding the socket");
		return 2;
	}




/***               start the client here                            ***/
	p_id=0;
	(void)signal(SIGINT,sig_hander);
	while(flag){
/***                                                               ***/

/***            generate one request                            ***/
		TTL = rand()%3+1;
		from= rand()%9;
		if (from == 0 || from == 1 || from == 2){
			To = rand()%7+3;
		}
		if (from ==3 || from == 4) {
			To=rand()%10;
			while(To == 3 || To ==4)
				To=rand()%10;
		}

		if (from == 5 || from == 6 || from == 7 || from == 8){	
			To=rand()%10;
			while(To == 5 || To == 6 || To == 7 || To == 8)
				To=rand()%10;
		}





		while (To==from)
			To=rand()%10;
		p_id++;
		printf("%d,%d\n",from,To);

 
		sprintf(buffer,"<%d> <%s> <%s> <%d> <%s>\n",p_id,addr_ip[from],addr_ip[To],TTL,payload);
		printf("<%d>,<%s>,<%s>,<%d>,<%s>\n",p_id,addr_ip[from],addr_ip[To],TTL,payload);
		printf("buffer:%s\n",buffer);
		sendto(s, buffer, strlen(buffer) + 1, 0, &si_other, sizeof(si_other));
                       
            
		if (To==9)
			InvD++;

		if (from == 0 || from == 1 || from == 2){
			if (To == 3 || To ==4)
				BtoA++;
			if (To == 5 || To == 6 || To == 7 || To == 8 )			
				BtoC++;
			}
		if (from ==3 || from == 4) {
			if (To == 0 || To == 1 || To == 2)
				AtoB++;
			if (To == 5 || To == 6 || To == 7 || To == 8)			
				AtoC++;
			}

		if (from == 5 || from == 6 || from == 7 || from == 8){	
			if (To == 3 || To ==4)
				CtoA++;
			if (To == 0 || To == 1 || To == 2)			
				CtoB++;
			}

/***         stop for two second for each two packets              ***/
		if (p_id%2 ==0)
			sleep(2);


/***          write to a file                                    ***/
		printf("%d,%d,%d,%d,%d,%d,%d\n",AtoB,AtoC,BtoA,BtoC,CtoA,CtoB,InvD);
		if (p_id%20 == 0){
			fseek(f,0,SEEK_SET);
			fprintf(f,"NetA to NetB:%d\n",AtoB);
			fprintf(f,"NetA to NetC:%d\n",AtoC);
			fprintf(f,"NetB to NetB:%d\n",BtoA);
			fprintf(f,"NetB to NetB:%d\n",BtoC);
			fprintf(f,"NetC to NetB:%d\n",CtoB);
			fprintf(f,"NetC to NetB:%d\n",CtoA);
			fprintf(f,"Invalid Destination:%d\n",InvD);
		
			}
	} 

	fseek(f,0,SEEK_SET);
	fprintf(f,"NetA to NetB:%d\n",AtoB);
	fprintf(f,"NetA to NetC:%d\n",AtoC);
	fprintf(f,"NetB to NetB:%d\n",BtoA);
	fprintf(f,"NetB to NetB:%d\n",BtoC);
	fprintf(f,"NetC to NetB:%d\n",CtoB);
	fprintf(f,"NetC to NetB:%d\n",CtoA);
	fprintf(f,"Invalid Destination:%d\n",InvD);




	fclose(f);
}






