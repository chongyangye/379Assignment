/*
CMPUT 379 Assignment 2a
Chongyang YE
Student ID:1264608
Unix ID: cye2
*/

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <strings.h>
#include <sys/un.h>
#include <stdlib.h>
#define BUFLEN 512
#define PORT 9931
#define IP 2130706433  /* 127.0.0.1 */

int flag=1;
int source, destination;
char buffer[BUFLEN];
char *payload = "Hello";
int sendAtoB=0, sendAtoC=0, sendBtoA=0, sendBtoC=0;
int sendCtoA=0, sendCtoB=0, invalied=0;
const char *addr_ip[10]={"192.168.192.4","192.168.192.6","192.168.192.10",
			"192.168.128.7","192.168.128.1","192.224.0.5","192.224.10.5",
			"192.224.0.7","192.224.15.6","168.130.192.01"};
static void usage()
{
	extern char * __progname;
	fprintf(stderr, "usage: %s portnumber\n", __progname);
	exit(0);
}

//signal handler
void sig_handler(int signo){
	flag =0;

}
//randomly generate the source ip and destination ip
void generate_packet(int packet_id){
    
    
    //random choose one of the ip address 
    source =rand()%9;
    destination = rand()%10;
    while(source == destination){
        destination = rand()%10;
    }
    
    //check the source and destination are the same clinet or not
    if(source ==0||source ==1||source==2){
        while(destination<=2){
            destination=rand()%10;
        }
    }else if(source ==3||source ==4){
        while(destination==3||destination==4){
            destination=rand()%10;
        }
    }else{
        while(destination>=5){
            destination=rand()%10;
        }
    }
    
    int ttl = rand()%4;
    sprintf(buffer,"%d,%s,%s,%d,%s\n",packet_id,addr_ip[source],addr_ip[destination],ttl,payload);
    printf("%d,%s,%s,%d,%s\n",packet_id,addr_ip[source],addr_ip[destination],ttl,payload);
    printf("buffer:%s\n",buffer);
}
//write to file
void update_to_file(FILE *fp){
    if(fp==NULL){
        fprintf(stderr,"Error opening file\n");
	    exit(0);
    }
    fseek(fp,0,SEEK_SET);
	fprintf(fp,"NetA to NetB:%d\n",sendAtoB);
	fprintf(fp,"NetA to NetC:%d\n",sendAtoC);
	fprintf(fp,"NetB to NetB:%d\n",sendBtoA);
	fprintf(fp,"NetB to NetB:%d\n",sendBtoC);
	fprintf(fp,"NetC to NetB:%d\n",sendCtoB);
	fprintf(fp,"NetC to NetB:%d\n",sendCtoA);
    fprintf(fp,"Invalid Destination:%d\n",invalied);
}

int main(int argc, char *argv[]){
    
    struct sockaddr_in si_me, si_other;
    int s, i, slen=sizeof(si_other);
    int port;
    char *ep;
    FILE *fp = fopen(argv[2], "w+");

    //check if the arguements take are 3 or not
    if(argc!=3){
        usage();
    }
    //get the port number
    port = strtoul(argv[1], &ep, 10);
    if ( ( s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) ) == -1 )
	{
		printf("Error in creating socket\n");
		exit(0);
	}

	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(IP);

	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(port);
	si_other.sin_addr.s_addr = htonl(IP); 


	if ( bind(s, (struct sockaddr*)&si_me, sizeof(si_me)) == -1 )
	{
		printf("Error in binding the socket\n");
		return 2;
	}
    srand(time(0));
	(void) signal(SIGINT, sig_handler);
	int packet_id = 1;
	//start to listen
	while(flag){
	    generate_packet(packet_id);
	    sendto(s, buffer, strlen(buffer) + 1, 0, (struct sockaddr*)&si_other, sizeof(si_other));
	    packet_id++;
	    //check source ip and destination ip add to counter
	    if (source==0||source==1||source==2){
	        if(destination==3 ||destination==4){
	            sendBtoA++;
	        }else{
	            if(source<=8){
	                sendBtoC++;
	            }else{
	                invalied++;
	            }
	        }
	    }else if(source ==3||source==4){
	        if(destination<=2){
	            sendAtoB++;
	        }else{
	            if(source<=8){
	                sendAtoC++;
	            }else{
	                invalied++;
	            }
	        }
	    }else{
	        if(source<=2){
	            sendCtoB++;
	        }else{
	            if(source<=4){
	                sendCtoA++;
	            }else{
	                invalied++;
	            }
	        }
	    
	    }
	    if (packet_id%2 ==0){
			sleep(2);
		}
	    if(packet_id%20==0){    
	        update_to_file(fp);
	    }
	}
	
	update_to_file(fp);
	
	fclose(fp);
	close(s);
	return 0;



}
