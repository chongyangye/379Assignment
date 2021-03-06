/*
CMPUT 379 Assignment 2b
Chongyang YE
Student ID:1264608
Unix ID: cye2
*/

#define BUFLEN 2048
#define LINELEN 512
#define CHUNKLEN 1024
#define _BSD_SOURCE
#include <sys/types.h>
#include <sys/time.h>
#include <sys/file.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <pwd.h>
#include <errno.h>
#include <sys/stat.h>
#include <err.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>           
#include <malloc.h>         
#define NOTFOUND        "File not found!\n"
#define NOTCOMPLETE     "Transmission not completed!\n"

struct bind_s {
    struct sockaddr_in      si;
    char                    *logFile_dir;
    char                    *document_dir;
    int                     ss;
};

typedef struct bind_s bind_t;

struct conn_s {
    struct sockaddr_in client;
    socklen_t       slen;
    char            buffer[BUFLEN];
    char            *rec_time;
    bind_t          *s;
};

typedef struct conn_s conn_t; 

static void kidhandler(int signum) {

	waitpid(WAIT_ANY, NULL, WNOHANG);
}

static void usage()
{
	extern char * __progname;
	fprintf(stderr, "usage: %s portnumber\n", __progname);
	exit(0);
}

void processingDone(FILE* endFile){
    if(endFile < 0){
		perror("CLIENT:\n");
		exit(1);
	}
}

void processing(conn_t *r){

	char libArtc[BUFLEN],newCon[CHUNKLEN];
	char *finish_transmission,*answerFile,*response;
    int	c, fileCounter;
    int complete = 0, notDone = 0;
    time_t	new_time;
	struct tm *time_information;
	char buffer[80];

	
	answerFile = malloc(1024 * sizeof(char));
	response = malloc(1024*sizeof(char));
	snprintf(libArtc, sizeof(libArtc), "%s/%s", r->s->document_dir, r->buffer);
	sprintf(answerFile, "%s %d %s %s", inet_ntoa(r->client.sin_addr),
			ntohs(r->client.sin_port), r->buffer, r->rec_time);
	memset(buffer, 0, 81);
	strcat(response,"file found\n");
	int checksum=1,jumpFile=1;
	FILE* article=fopen(libArtc, "r");
	if(article == NULL) {
		fprintf(stderr, NOTFOUND );
		sendto(r->s->ss, NOTCOMPLETE, strlen(NOTCOMPLETE), 0,
				(struct sockaddr*)&r->client, sizeof(r->client));
		strcat(answerFile, " file not found\n");
	}else{
	//not null
	    if(article!=NULL && jumpFile==1){
	        fprintf(stderr, "Can Access.\n");
	    }
		if(access( r->s->document_dir, R_OK) != -1){
		
			while(!feof(article)){
                bzero(&newCon, sizeof(newCon));
				for(fileCounter=0; fileCounter < 1024; fileCounter++){
					c=fgetc(article);
					if(c!=EOF){
						strcat(newCon, (const char*)&c);
					}
					if(feof(article)){
						if(fileCounter == 0)
							strcat(newCon, "$");
						break;
					}
				}
				notDone = sendto(r->s->ss, newCon, sizeof(newCon), 0,
							(struct sockaddr*)&r->client,sizeof(r->client));
			    jumpFile=0;			
				if(notDone < 0){
					fprintf(stderr, NOTCOMPLETE);
					break;
				}
			}
			if(notDone >= 0&&checksum==1){
			    time( &new_time );
	            time_information = localtime( &new_time );
	            strftime(buffer,80,"%a %d %b %Y %X %Z", time_information);
				fprintf(stdout, "Successfilly\n");
				finish_transmission = strdup(buffer);
				complete = 1;
				
			}
		} else {
			fprintf(stderr, "CAN NOT READ.\n");
		}
		//done with file
		if (complete ==1){
		    jumpFile = 1;
		}
	}
		
	FILE* fileJustRead = fopen(r->s->logFile_dir, "a");
	flock(fileno(fileJustRead), LOCK_EX);
	processingDone(fileJustRead);
	if(complete == 1)
		fprintf(fileJustRead,"%s %s\n", answerFile, finish_transmission);
	else
		fprintf(fileJustRead,"%s  Transmission Fail\n", answerFile);
	flock(fileno(fileJustRead), LOCK_UN);
	fclose(fileJustRead);
	free(response); 
	free(answerFile);
}




int main(int argc,char *argv[])
{
	struct	sigaction sa;
	time_t	second_new_time;
	char	*ep;
   	pid_t	pid;
   	int port;
    int errno;
    struct tm *new_time_information;


  	if (argc != 4){
		usage();
		errno = 0;
	}
	
    strtoul(argv[1], &ep, 10);
 	if (*argv[1] == '\0' || *ep != '\0') {
		printf("WRONG PORT NUMBER\n");
		usage();
	}
    char	buf[80];
	memset(buf, 0, 81);
    bind_t *s = malloc(sizeof(bind_t));
	bzero(s, sizeof(bind_t));

	port = strtoul(argv[1], &ep, 10);
	s->document_dir = argv[2];
	s->logFile_dir = argv[3];
	
	FILE*	file = fopen(argv[3], "a");
	if (file < 0) {
		perror("CLIENT:\n");
		exit(EXIT_FAILURE);
	}
	fclose(file);
    //Daemonise, run background
    if (daemon(1, 0) == -1)
        err(1, "daemon() failed");
	

	s->si.sin_family = AF_INET;
	s->si.sin_port = htons(port);
	s->si.sin_addr.s_addr = htonl(INADDR_ANY);

	if ((s->ss = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		err(1,"Client: cannot connect socket\n");

	if (bind(s->ss, (struct sockaddr *)&s->si, sizeof(s->si)) == -1)
		err(1,"Binding Error\n");

	/* Catching SIGCHLD to make sure we have no zombies children */
	sa.sa_handler = kidhandler;
		sigemptyset(&sa.sa_mask);

	/*
	* allow system calls(eg. accept) to be restarted 
	* if they get interrupted by a SIGCHLD
	*/
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1)
		err(1, "sigaction failed\n");

	printf("Start to listen to port: %d\n", ntohs(s->si.sin_port));
	
	while(1){
        conn_t *c = malloc(sizeof(conn_t));
        bzero(c, sizeof(conn_t));
        
		c->slen = sizeof(c->client);
        c->s = s;

        if((recvfrom(c->s->ss, c->buffer, sizeof(c->buffer), 0,
            (struct sockaddr*)&c->client, &c->slen)) != -1){
			pid = fork();
			if (pid == 0){
			    time( &second_new_time );
	            new_time_information = localtime( &second_new_time );
	            strftime(buf,80,"%a %d %b %Y %X %Z", new_time_information);
				c->rec_time = strdup(buf);
				processing(c);
				close(s->ss);
				exit(0);				
			} else if (pid == -1) {
				err(1, "Can not Fork");
			}
		}else{
            err(errno, "Fail");
        }
        free(c);
	}
}



