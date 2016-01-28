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

int main(){
int i;
for(i=0;i<100;i++){
    printf("%d",rand_int(1,10));

}
return 0;
}
