#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>

void handler (int sig, siginfo_t *sip, void *uap)
{
    printf("Reception signal %d, val = %d \n", sig,sip->si_value.sival_int);
}

int main(int argc, char * argv[])
{
struct sigaction sig; 
union sigval val;
int cpt;
	sig.sa_flags=SA_SIGINFO;
	sig.sa_sigaction=handler;
	sigemptyset(&sig.sa_mask);
	if(sigaction(SIGRTMIN,&sig,NULL)<0) perror("sigaction");
	for(cpt=0;cpt<5;cpt++) {
		struct timespec delai;
		delai.tv_sec=1;
		delai.tv_nsec=0;
		val.sival_int=cpt;
		sigqueue(getpid(),SIGRTMIN,val);
		nanosleep(&delai,NULL);
	}
	return 0;
}
