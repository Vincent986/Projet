#include <stdio.h>
#include <signal.h>
#include <time.h>

int go=1;
void trop_tard(int sig)
{
	printf("Signal %d recu\n",sig);
	go=0;
}

int main(int argc, char * argv[])
{
timer_t monTimer;
struct sigaction sig;
struct itimerspec ti;
int capacite=1;
	timer_create(CLOCK_REALTIME,NULL,&monTimer);
	sig.sa_flags=SA_RESTART;
	sig.sa_handler=trop_tard;
	sigemptyset(&sig.sa_mask);
	sigaction(SIGALRM,&sig,NULL);
	ti.it_value.tv_sec=capacite;
	ti.it_value.tv_nsec=0;
	ti.it_interval.tv_sec=0;    /* ici le timer n'est pas */
	ti.it_interval.tv_nsec=0;   /* automatiquement réarmé */
	timer_settime(monTimer,0,&ti,NULL);
	printf("Debut capacite\n");
	while (go>0) printf("Je travaille ... \n");
	printf("Debloquee par timer : echeance rates ..\n");
	return 0;
}
