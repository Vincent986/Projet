#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem;

void* th(void* arg)
{
    printf("thread %lu en attente\n",(unsigned long)pthread_self());
    sem_wait(&sem);
    printf("thread %lu debloqué\n",(unsigned long)pthread_self());
    return NULL;
}

int main (int argc, char* argv[])
{
pthread_t id;
struct timespec delai;
    sem_init(&sem,0,0);
    pthread_create(&id,NULL,th,NULL);
    delai.tv_sec=4;
    delai.tv_nsec=0;
    nanosleep(&delai,NULL);
    printf("thread principal %lu : liberation de l'autre thread \n",
			(unsigned long)pthread_self());
    sem_post(&sem);
    pthread_exit(NULL);
    return 0;
}

