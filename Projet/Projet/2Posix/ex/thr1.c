#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

void* th(void* arg)
{
	printf("Je suis le thread %lu processus %d\n",
		(unsigned long)pthread_self(),getpid());
	pthread_exit(NULL);
	return NULL;
}


int main(int argc, char* argv[])
{
pthread_t id1 ,id2;
	pthread_create(&id1,NULL,th,NULL);
	pthread_create(&id2,NULL,th,NULL);
	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	printf("Fin du thread principal %lu processus %d\n",
        	(unsigned long)pthread_self(),getpid());
	pthread_exit(NULL);
}
