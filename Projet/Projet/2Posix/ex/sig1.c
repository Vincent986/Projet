#include <stdio.h>
#include <signal.h>

void handler(int sig)
{
	printf("Signal %d recu\n",sig);
}
int main(int argc, char * argv[]) 
{
struct sigaction sig;
    sig.sa_flags=SA_RESTART; 
    sig.sa_handler=handler; 
    sigemptyset(&sig.sa_mask);
    sigaction(SIGUSR1,&sig,NULL);
    while(1); /* le programme travaille !! */
}
