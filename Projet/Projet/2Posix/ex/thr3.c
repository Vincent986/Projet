#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_key_t cd_key;
int pthread_cd_init(void)
{
	return pthread_key_create(&cd_key, NULL);
}

char* pthread_get_cd(void)
{
	return (char*)pthread_getspecific(cd_key);
}

int pthread_set_cd(char* cd)
{
char * mycd = (char*)malloc(sizeof(char)*100);
	strcpy(mycd,cd);
	return pthread_setspecific(cd_key,mycd);
}

int main(int argc, char* argv [])
{
	pthread_cd_init();
	pthread_set_cd("/rep1/rep2");
	printf("Mon repertoire courant est %s\n",pthread_get_cd());
	return 0;
}
