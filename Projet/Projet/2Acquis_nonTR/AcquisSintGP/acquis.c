/* programme qui fait de l'acquisition sur l'horloge sans utiliser Xenomai
   mais en utilisant les outils "orientes TR" comme setitimer 
*************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

#define LBUF 200

int NOB=0; /* Ce numéro indique a la tache NTR quel buffer utiliser !*/
struct timeval Buf[LBUF*2], *BUF1, *BUF2;
int IndB=0;

void fct_acq(int S)
{
   if (S == SIGALRM) {
      gettimeofday(&Buf[IndB], NULL);
      IndB++;
      if (IndB == LBUF) NOB=1;
      if (IndB == LBUF*2){
          NOB=2;
          IndB =0;
      }
   }
}

int main(int N, char *P[])
{
struct itimerval iT;

    BUF1 = Buf;
    BUF2 = Buf+LBUF;
    signal(SIGALRM, fct_acq);

    /* initialisation des parametres du timer ! */
    /* le delai : */
    iT.it_value.tv_sec = 1; /* valeur en secondes */
    iT.it_value.tv_usec = 0; /* valeur en micro-secondes */
    /* la periode */
    iT.it_interval.tv_sec = 0; /* valeur en secondes */
    iT.it_interval.tv_usec = 10000; /* valeur en micro-secondes */
    /* initialisation du timer */
    setitimer(ITIMER_REAL, &iT, NULL);
    /* attend le signal pour sauvegarder un buffer */
    while (1) {
      if (NOB) {
         printf("Sauvegarde du buffer no %d !\n", NOB);
         NOB=0;
      }
    }
    return 0;
}


