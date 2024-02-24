#include <stdio.h>
#include <signal.h>
#include <time.h>

int go=1;
void trop_tard(int sig, siginfo_t *info, void *uap)
{
	printf("Reception signal %d, capacite = %d depassee \n",
		sig, info->si_value.sival_int);
	go=0;
}

int main(int argc, char * argv[])
{
timer_t monTimer;
struct sigevent event; 
struct sigaction sig; 
struct itimerspec ti;
int capacite = 1;
	event.sigev_notify=SIGEV_SIGNAL;
	event.sigev_value.sival_int=capacite;
	event.sigev_signo=SIGRTMIN;
	timer_create(CLOCK_REALTIME,&event,&monTimer);
	sig.sa_flags=SA_SIGINFO; 
	sig.sa_sigaction=trop_tard; 
	sigemptyset(&sig.sa_mask); 
	sigaction(SIGRTMIN,&sig,NULL);
	ti.it_value.tv_sec=capacite; 
	ti.it_value.tv_nsec=0;
	ti.it_interval.tv_sec=0;	/* timer non réarmé */
	ti.it_interval.tv_nsec=0; 
	timer_settime(monTimer,0,&ti,NULL);
	printf("Debut capacite\n"); 
	while(go>0) printf("Je travaille ...\n");
	printf("Debloquee par timer : echeance ratee ..\n");
	return 0;
}
