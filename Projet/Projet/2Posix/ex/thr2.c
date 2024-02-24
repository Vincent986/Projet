#include <pthread.h>
#include <stdio.h>

void* th(void* arg)
{
	printf("Je suis la thread %lu\n", (unsigned long)pthread_self ());
	return NULL;
}

int main(int argc, char* argv[])
{
int i;
pthread_t id;
pthread_attr_t Attr;
struct sched_param Param;
    pthread_attr_init(&Attr);
    pthread_attr_setdetachstate(&Attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setschedpolicy(&Attr, SCHED_FIFO);
    Param.sched_priority=1;
    pthread_attr_setschedparam(&Attr , &Param);
    for(i=1;i<10;i++)
           pthread_create(&id, &Attr, th, NULL);
    return 0;
}
