#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <aio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


void io_terminee(int sig, siginfo_t *info, void *uap)
{
	struct aiocb* io=(struct aiocb*)info->si_value.sival_ptr; 
	printf("E/S terminee sur le descripteur %d\n", io->aio_fildes);
	if(aio_error(io)!=EINPROGRESS)
		printf("Nb caracteres ecrits %ld\n", (long)aio_return(io));
	exit(0);
}

int main(int argc, char * argv[])
{
char msg[100];
int fd;
struct sigaction sig; 
struct aiocb io;
	fd=open("fic.txt",O_CREAT | O_SYNC | O_WRONLY);
	sig.sa_flags=SA_SIGINFO; 
	sig.sa_sigaction=io_terminee; 
	sigemptyset(&sig.sa_mask); 
	sigaction(SIGRTMIN,&sig,NULL);
	strcpy(msg,"Hello word !");
	io.aio_buf=msg;
	io.aio_offset=0;
	io.aio_fildes=fd;
	io.aio_nbytes=strlen(msg); 
	io.aio_reqprio=0;
	io.aio_sigevent.sigev_notify=SIGEV_SIGNAL;
	io.aio_sigevent.sigev_value.sival_ptr=&io; 
	io.aio_sigevent.sigev_signo=SIGRTMIN;
	aio_write(&io);
	printf("Emission de l'ecriture\n");
	while(1);
}
