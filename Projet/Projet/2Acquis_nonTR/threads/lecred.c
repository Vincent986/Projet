/* exemple d'implementation de l'algo lecteurs/redacteurs avec POSIX 1003.1 */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

/* le semaphore du fichier */
sem_t semFic;

void affiche(char * m, int N)
{
   printf("PID:%d THID:%ld : %s %d !\n",getpid(), (unsigned long)pthread_self(),
     m, N);
}

void * lecteur(void * p)
{
int Nb=10;
long no;
   no = (long)p;
   affiche("Je suis le lecteur no ",no);
   while (Nb) {
      sem_wait(&semFic);
      affiche("Le fichier est utilise par le lecteur no ",no);
      sleep((unsigned int)(no*2));
      sem_post(&semFic);
      Nb--;
      sleep(1);
   }
   return NULL;
}

int main(int N, char **P)
{
pthread_t thid;
long il=1, ir=1;
int err;

   affiche("Debut du programme : thread ", 0);
   /* initialisation des mutex et semaphores */
   sem_init(&semFic,0,1);
   /* on lance 4 lecteurs */
   for (;il<=4; il++) {
     if ((err=pthread_create(&thid, NULL, lecteur, (void*)il)) != 0) {
      fprintf(stderr,"Erreur %d avec pthread_create !\n",err);
      return 1;
     }
   }
   affiche("Fin du programme : thread ", 0);
   pthread_exit(NULL);
   /* return 0; ceci termine le processus !! */
}

