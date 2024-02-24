#include <pthread.h>
#include <mqueue.h>
#include <stdio.h>
#include <string.h>

#define NOMFILE "/tmp/mafifo"

void* consommateur(void* arg) {
mqd_t id;
char buff [100] ;
	id=mq_open(NOMFILE,O_RDONLY);
	mq_receive(id,buff,100,NULL);
	printf("msg = %s\n",buff);
	mq_unlink(NOMFILE);
	pthread_exit(NULL);
}

struct mq_attr attr;

int main(int argc, char* argv[])
{
pthread_t tid;
mqd_t id;
char buff [100];
	attr.mq_maxmsg=100;
	attr.mq_flags=0;
	attr.mq_msgsize=100;
	id=mq_open(NOMFILE,O_CREAT|O_WRONLY,00644,&attr);
	strcpy(buff,"coucou");
	/* Emission avec la priorite 1 (de 0 a MQ_PRIO MAX) */
	mq_send(id,buff,100,1);
	pthread_create(&tid,NULL,consommateur,NULL);
	pthread_exit(NULL);
}

