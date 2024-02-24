#include <stdio.h>
#include <pthread.h>

int y=2, x=0;
pthread_mutex_t mut;
pthread_cond_t cond;

void* th(void* arg) 
{
int cont=1;
    while(cont)  {
       pthread_mutex_lock(&mut);
       x++;
       printf("x++\n");
       if (x > y)  {
	pthread_cond_signal(&cond);
	cont=0;
       }
       pthread_mutex_unlock(&mut);
    }
    return NULL;
}

int main(int argc, char* argv[])
{
pthread_t id;
	pthread_mutex_init(&mut,NULL);
	pthread_cond_init(&cond,NULL);
	pthread_create(&id,NULL,th,NULL);
	pthread_mutex_lock(&mut);
	while (x <= y) pthread_cond_wait(&cond, &mut);
	printf("x>y est vrai\n");
	pthread_mutex_unlock(&mut);
	return 0;
}
