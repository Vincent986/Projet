/* exemple simple de création d'un thread avec passage de parametre */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

struct Elt {
   int no;
   float v1;
   float v2;
   float v3;
};

void affiche(char * m, int N)
{
   printf("PID:%d THID:%ld : %s %d !\n",getpid(), (unsigned long)pthread_self(),
     m, N);
}

void * fct(void * p)
{
struct Elt *E;
   E = (struct Elt *)p;
   affiche("Je suis le thread no ",E->no);
   E->v3 = E->v1 * E->v2;
   return (void*)E;
}

int main(int N, char **P)
{
struct Elt T[2], *E;
void *R;
pthread_t thid,thid2;
int err;

   affiche("Debut du programme : thread ", 0);
   /* initialisation de mes structures Elt */
   T[0].no=1;
   T[0].v1=56.789;
   T[0].v2=23.456;
   T[1].no=2;
   T[1].v1=34.567;
   T[1].v2=89.123;
   if ((err=pthread_create(&thid, NULL, fct, (void*)(&T[0]))) != 0) {
      fprintf(stderr,"Erreur %d avec pthread_create !\n",err);
      return 1;
   }
   if ((err=pthread_create(&thid2, NULL, fct, (void*)(&T[1]))) != 0) {
      fprintf(stderr,"Erreur %d avec pthread_create !\n",err);
      return 1;
   }
   pthread_join(thid,&R);
   E = (struct Elt *)R;
   printf("Le thread no %d a trouve que %f * %f = %f !\n",E->no,E->v1,E->v2,E->v3);
   pthread_join(thid2,&R);
   E = (struct Elt *)R;
   printf("Le thread no %d a trouve que %f * %f = %f !\n",E->no,E->v1,E->v2,E->v3);
   affiche("Fin du programme : thread ", 0);
   return 0;
}

