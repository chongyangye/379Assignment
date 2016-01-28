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
#include <strings.h>
#include <stdlib.h>
#include <signal.h>
#define BUFLEN 512
#define IP 2130706433  /* 127.0.0.1 */
int flag=1;
int expir=0, unrout=0,deliv=0,RB=0,RC=0;

typedef struct{
    int prefix_len;
    char ip_address[50];
    char next[50];
} router_tab ;

router_tab router_table[10];

static void usage()
{
	extern char * __progname;
	fprintf(stderr, "usage: %s portnumber\n", __progname);
	exit(0);
}

void sig_handler(int sig)
{
	flag = 0;


	
}

in_addr_t convert_ip(int n){
	int r = 0;
	int i;
	for( i = 0; i< n; i++){
		r = r | (1 << i);
	} 
	r = r << (32-n);
	return (in_addr_t) r;
}
//read the file
void read_table(FILE *read_rt){
    if(read_rt == NULL){
        fprintf(stderr,"cannot open RT file\n");
        exit(0);
    }
    char line[256];
    bzero(router_table,sizeof(router_tab));
    int i =0;
    while(fgets(line,sizeof(line),read_rt)){
    
        if(line[0] =='\r' || line[0]=='\n' || line[0]=='\0'){
            continue;
        }else{
            sscanf(line,"%s%d%s",router_table[i].ip_address, &router_table[i].prefix_len, router_table[i].next);

            i++;
        }
    }
}

//check the router
void process_ip(in_addr_t net_ip, int nett_ip[]){
    int i,check=1;
    for(i=0;i<3;i++){
        in_addr_t received_ip = convert_ip(router_table[i].prefix_len);
        if((nett_ip[i]&(int)received_ip)==0){
            if(strncmp(router_table[i].next,"0",1)==0){
                deliv++;
            }else if(strncmp(router_table[i].next,"RouterB",7)==0){
                RB++;
            }else if(strncmp(router_table[i].next,"RouterC",7)==0){
                RC++;
            }
            check=0;
            break;
        }
    }
    if(check){
        unrout++;
        
    }


}
//get the buffer
void process_receive(char *buff){
    char neww_ip[BUFLEN];
    char *packet;
    packet =strtok(buff,",");
    int index ,ttl,j,nett_ip[3];
    for(index=0;index<5;index++){
        if(index ==2){
            strncpy(neww_ip,packet,512);
        }
        if(index ==3){
            ttl=atoi(packet);
            if(ttl<=1){
                expir++;
            }else{
                in_addr_t net_ip = inet_network(neww_ip);
                for(j=0;j<3;j++){
                    nett_ip[j]=(intptr_t)router_table[j].next ^ (intptr_t)net_ip;
                }
                process_ip(net_ip,nett_ip);
            }
        }
        packet=strtok(NULL,",");
    }




}



int main(int argc, char *argv[]){
    char buffer[BUFLEN], *ep;
    int port;
    struct sockaddr_in sockname, client;

    int s, client_len = sizeof(client);


    if(argc!=4){
        usage();
    }
    FILE *RT = fopen(argv[2],"r");
    FILE *STAT = fopen(argv[3],"w+");

    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
		printf("Error in creating socket\n");
		exit(0);
	}
	port = strtoul(argv[1], &ep, 10);
	memset((char*)&sockname, 0, sizeof(sockname));
	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(port);

	sockname.sin_addr.s_addr = htonl(IP);


    if (bind(s, (struct sockaddr*)&sockname, sizeof(sockname)) == -1){
		printf("bind failed\n");
		exit(0);
    }

    //read table
    read_table(RT);


    (void) signal(SIGINT, sig_handler);
    int count =1;
    //start to listen
    while(flag){
        if (recvfrom(s,buffer,BUFLEN,0,(struct sockaddr *)&client,(socklen_t*)&client_len)!=-1){
            printf("Received packet from %s:%d  NO: %s\n",inet_ntoa(client.sin_addr), ntohs(client.sin_port),buffer);
            process_receive(buffer);
            if(count%20==0){
                //update
                if(STAT==NULL){
                    fprintf(stderr,"Error opening STAT file\n");
		            exit(0);
                }
                //write to file
                fseek(STAT,0,SEEK_SET);
                fprintf(STAT,"expirired packets: %d\n",expir);
	            fprintf(STAT,"Unroutable packets: %d\n",unrout);
	            fprintf(STAT,"Delivered direct: %d\n",deliv);
	            fprintf(STAT,"Router B: %d\n",RB);
	            fprintf(STAT,"Router C: %d\n",RC);
                
            }
            count++;
        }
    
    
    }
    //write to file if intrrupt
    fseek(STAT,0,SEEK_SET);
    fprintf(STAT,"expirired packets: %d\n",expir);
	fprintf(STAT,"Unroutable packets: %d\n",unrout);
	fprintf(STAT,"Delivered direct: %d\n",deliv);
	fprintf(STAT,"Router B: %d\n",RB);
	fprintf(STAT,"Router C: %d\n",RC);
                
    
    
    
    fclose(RT);
    fclose(STAT);
    close(s);
	return 0;


}
