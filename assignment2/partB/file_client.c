/*
CMPUT 379 Assignment 2b
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
#include <stdlib.h>
#include <sys/un.h>
#define BUFLEN 1024

#define PORT 9092
#define IP 2130706433  /* 127.0.0.1 */
//check if the arguments is not 4 exit
static void usage()
{
	extern char * __progname;
	fprintf(stderr, "usage: %s portnumber\n", __progname);
	exit(0);
}


int main(int argc, char *argv[]) {
	struct sockaddr_in si_me, si_other;
	int s, slen=sizeof(si_other);
	char *ep, buffer[BUFLEN];
	in_addr_t ip;
	int port;
	int check;
	char content[1024][1024];
	memset(content,0,1048576);

	if (argc < 4) {
		usage();
	}
	ip=inet_network(argv[1]);
    port = strtoul(argv[2], &ep, 10);

	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1 )
	{
		printf("Error in creating socket");
		return 1;
	}

	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(ip); 

	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(port);
	si_other.sin_addr.s_addr = htonl(ip); 


	if (bind(s,(struct sockaddr*)&si_me, sizeof(si_me)) == -1 )
	{
		printf("Error in binding the socket");
		return 2;
	}
    printf("Send");
	sendto(s,argv[3],strlen(argv[3])+1,0,(struct sockaddr*)&si_other, sizeof(si_other));

    //wait for time
	struct timeval tv;	
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));

    //start to listen
	while(1){
		if ((check=recvfrom(s, buffer, BUFLEN, 0, (struct sockaddr*)&si_other, (socklen_t*)&slen)) != -1) {

			setsockopt(s,SOL_SOCKET,SO_RCVTIMEO,(char *)&tv,sizeof(struct timeval));

			if((check < 17) && (strncmp(buffer, "File can not found!\n", 20) == 0)){
				printf("File cannot find.\n");
				break;
			} else {
				if((check < 2) && (strncmp(buffer, "$", 1) == 0)){
					printf("successfully.\n");
					break;
				} else {
					write(1, buffer, check);
				}
				if(strlen(buffer) < 1024){
				    if(strlen(buffer)==0){
					    printf("Not successfully.\n");
					    break;
					}else{
					    break;
					}
				}
			}
			continue;
		}
		break;
	}
	
	close(s);
	return 0;
}
    
