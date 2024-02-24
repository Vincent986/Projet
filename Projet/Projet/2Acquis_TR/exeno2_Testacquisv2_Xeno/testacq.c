/* exemple de taches periodiques
   avec detection des overruns !!
******************************* */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <sys/mman.h>

#define VERSION "1.4"

#ifdef CONFIG_XENO_COBALT

#include <native/task.h>
#include <native/timer.h>
#include <native/sem.h>
#define  PER 20500 /* periode d'acquisition en nanosecondes */
#define  NBV 8192  /* Nb de valeurs avant 1000 */
#define NBB 30       /* nb de boucles dans main */

#else /* MERCURY */

#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/sem.h>
#define PER 200000000 /* periode d'acquisition en nanosecondes */
#define NBV 100       /* Nb de valeurs */
#define NBB 6         /* nb de boucles dans main */

#endif


static RT_SEM sem_id, sem_id2;
static void * B, *C;
static int Trace=0, STOP=0, iB=0, nB=0, eB=0, oR=0;

static void f_rec(void *arg)
{
RTIME *d1, *d2, *s1, *s2;
size_t t;
int err;

  d1 = (RTIME*)C;
  d2 = d1+NBV;
  s1 = (RTIME*)B;
  s2 = s1+NBV;
  t = (size_t)(NBV*sizeof(RTIME));
  while (1==1) {
     if ((err=rt_sem_p(&sem_id,TM_INFINITE))!=0) {
       fprintf(stderr, "ERREUR rt_sem_p %d!\n",err); return;
     }
     // if (iB == 1) bcopy((void*)s1,(void*)d1,t);
     // else  { bcopy((void*)s2,(void*)d2,t); nB++; }
     if (iB == 1) memcpy((void*)d1,(void*)s1,t);
     else  { memcpy((void*)d2,(void*)s2,t); nB++; }
     iB=0;
     if (STOP) break;
     if ((err=rt_sem_v(&sem_id2))!=0)
          fprintf(stderr,"ERREUR rt_sem_v 2 %d\n",err);
  }
  if (Trace) printf("Fin Recuperation !\n");
}

static void f_acq(void *arg)
{
int err;
unsigned long int out=0;
RT_TIMER_INFO timer_info;
RTIME *d, *f1, *f2;

  d = (RTIME*)B;
  f1 = d+NBV;
  f2 = f1+NBV;

  if (rt_task_set_periodic(
      NULL,   /* la tache courrante */
      TM_NOW, /* delai de mise en place, TM_NOW = none */
      PER    /* valeur en nanosecondes */) != 0)
  {
    fprintf(stderr,"rt_task_set_periodic error\n");
    return;
  }

  rt_task_wait_period(&out); /* on ignore le 1er tour pour partir ok */
  rt_task_wait_period(&out); /* on ignore le 2er tour pour partir ok */
  while (1==1) {
    if (STOP) {
       if ((err=rt_sem_v(&sem_id))!=0)
          fprintf(stderr,"ERREUR rt_sem_v %d\n",err);
       break;
    }
    rt_timer_inquire(&timer_info);
    *d++ = timer_info.date;
    if (d==f1) {
       if (iB) eB++;
       iB=1;
       if ((err=rt_sem_v(&sem_id))!=0)
          fprintf(stderr,"ERREUR rt_sem_v %d\n",err);
    }
    if (d==f2) {
       if (iB) eB++;
       iB=2;
       if ((err=rt_sem_v(&sem_id))!=0)
          fprintf(stderr,"ERREUR rt_sem_v %d\n",err);
       d = (RTIME*)B;
    }
    out = 0;
    /* Retour en primary mode a cause d'un appel systeme Xenomai */
    switch (rt_task_wait_period(&out)) {
      case 0:
      case -ETIMEDOUT:
        if (out) {
           // fprintf(stderr, "Overrun: %lu cycles perdus\n",out);
           oR += out;
        }
        break;
      default:
        fprintf(stderr,"ERREUR rt_task_wait_period !\n"); /* 2nd. mode */
        return;
    }
  }
  if (Trace) printf("Fin Acquisition !\n");
}

void informe(char *p, char *m)
{
   fprintf(stderr,"Version %s\nErreur : %s !\nUtilisation: %s [-t] [-n999]\n\t-t : trace 1 point a chaque buffer rempli + messages fin des taches TR.\n\t-n: 999 = nb de buffers.\n",VERSION, m, p);
   exit(1);
}

int main (int N, char*P[])
{
RT_TASK t_id, t_id2;
int n, i, err, *ti;
int opt, nbb=NBB;
RT_TIMER_INFO timer_info;
time_t t1, t2;

  /* verification parametre */
  while ((opt = getopt(N,P, "tn:")) != -1) {
     switch(opt) {
     case 't' :
          Trace = 1;
          break;
     case 'n' :
          nbb = atoi(optarg);
          break;
     default :
           informe(P[0],"parametre non valable");
     }
  }
  if (nbb <= 0) informe(P[0], "valeur non numerique");
  if (N > optind) informe(P[0],"parametre supplementaire inutile");
/*
  printf("Trace=%d nbb=%d optind=%d\n",Trace,nbb,optind);
  return 0;
*/
  time(&t1);
  mlockall(MCL_CURRENT | MCL_FUTURE); /* pour ne pas swapper */
  if (nice(-20) == -1) perror("nice");


  /* creation buffers */
  n = sizeof(timer_info.date);
  if ((B = malloc(NBV*n*2)) == NULL) {
     perror("malloc"); return 1;
  }
  if ((C = malloc(NBV*n*2)) == NULL) {
     perror("malloc"); return 1;
  }
  if ((ti = (int*)malloc(sizeof(int)*3*nbb)) == NULL) {
     perror("malloc ti"); return 1;
  }

  /* creation semaphores */
  if (rt_sem_create(&sem_id, "monSem", 0, S_FIFO) != 0) {
    fprintf(stderr,"ERREUR rt_sem_create !\n"); return 1;
  }
  if (rt_sem_create(&sem_id2, "monSem2", 0, S_FIFO) != 0) {
    fprintf(stderr,"ERREUR rt_sem_create 2!\n"); return 1;
  }

  if ((n=rt_task_spawn(&t_id,"taskAcq",0,99,T_JOINABLE,&f_acq,NULL))!=0) {
    fprintf(stderr,"ERREUR rt_task_spawn %d!\n",n);
    return 1;
  }
  if ((n=rt_task_spawn(&t_id2,"taskRecup",0,80,T_JOINABLE,&f_rec,NULL))!=0) {
    fprintf(stderr,"ERREUR rt_task_spawn 3 %d!\n",n);
    return 1;
  }
  if ((n=rt_task_shadow(NULL,"taskMain",30,0))!=0) {
    fprintf(stderr,"ERREUR rt_task_shadow %d!\n",n);
    return 1;
  }

  n=0;
  while (n<nbb) {
     if ((err=rt_sem_p(&sem_id2,TM_INFINITE))!=0) {
       fprintf(stderr, "ERREUR rt_sem_p 2 %d!\n",err); return 1;
     }
     if (Trace) { printf("."); fflush(stdout); }
     i=n*3;
     ti[i] = nB;
     ti[i+1] = eB;
     ti[i+2] = oR;
     if (oR > NBV) break;
     n++;
  }
  STOP=1;
  time(&t2);
  if (Trace) printf("\n");
  /* recap */
  n = sizeof(timer_info.date);
  printf("Test : Periode=%g ms, Frequence=%g hz.\n",
    (double)PER/(double)1000000, (double)1000000000/(double)PER );
  printf("Un elt = %d octets. Acquisition de %d buffers de %d elts !\n",
    n, nbb,NBV);
  n=0;
  while (n<nbb) {
     i=n*3;
     if ((ti[i+1]>0) || (ti[i+2]>0))
        printf("Transfert %d (nbelt %d) retards recup: %d, overruns %d !\n",
               n+1,ti[i],ti[i+1],ti[i+2]);
     if (ti[i+2] > NBV) {
        printf ("Abandon ...\n"); break;
     }
     n++;
  }
  if ((ti[i+1]>0) || (ti[i+2]>0))
     printf ("ERREURS : %d retards et %d overruns\n",ti[i+1],ti[i+2]);
  printf("Temps total : %d s, CPU : %g s\n",(int)(t2-t1),(double)clock()/(double)CLOCKS_PER_SEC);
  rt_task_join(&t_id);
  rt_task_join(&t_id2);
  return 0;
}

