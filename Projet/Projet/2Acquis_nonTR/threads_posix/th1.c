/* exemple simple de création d'un thread */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

void affiche(char * m)
{
   printf("PID:%d THID:%ld : %s\n",getpid(), (unsigned long)pthread_self(),m);
}

void * fct(void * p)
{
   affiche("Je suis le thread !");
   return NULL;
}

int main(int N, char **P)
{
pthread_t thid;
int err;

   affiche("Debut du programme");
   if ((err=pthread_create(&thid, NULL, fct, NULL)) != 0) {
      fprintf(stderr,"Erreur %d avec pthread_create !\n",err);
      return 1;
   }
   pthread_join(thid,NULL);
   affiche("Fin du programme");
   return 0;
}

