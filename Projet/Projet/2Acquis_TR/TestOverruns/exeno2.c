/* exemple de taches periodiques
   avec detection des overruns !!
******************************* */
#include <stdio.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/timer.h>

#define C1SEC 1000000000 /* 1 Sec en nano-sec */

static void f_task(void *arg)
{
volatile unsigned int i=0;
unsigned int n=0;
unsigned long int out=0;
unsigned long long int start = 0;
RT_TIMER_INFO timer_info;

  if (rt_task_set_periodic(
      NULL,   /* la tache courrante */
      TM_NOW, /* delai de mise en place, TM_NOW = none */
      C1SEC    /* valeur en nanosecondes */) != 0)
  {
    printf("rt_task_set_periodic error\n");
    return;
  }

  rt_timer_inquire(&timer_info);
  start = timer_info.date/C1SEC;

  while (n<30) {
    rt_timer_inquire(&timer_info);

    /* Affiche un message toutes les secondes */
    /* secondary mode */
    printf("Message #%u t= %llu s\n",n,timer_info.date/C1SEC - start);
    n++;

    if (n==3) /* simulation d'un gros calcul pour la boucle 3 */
      for (i=0;i<C1SEC;++i);

    out = 0;
    /* Retour en primary mode a cause d'un appel systeme Xenomai */
    switch (rt_task_wait_period(&out)) {
      case 0:
      case -ETIMEDOUT:
        if (out) printf("Overrun: %lu cycles perdus\n",out); /* 2nd. mode */
        break;
      default:
        printf("ERREUR rt_task_wait_period !\n"); /* 2nd. mode */
        return;
    }
  }
}

int main (int N, char*P[])
{
RT_TASK t_id;
int n;

  mlockall(MCL_CURRENT | MCL_FUTURE); /* pour ne pas swapper */

  if ((n=rt_task_spawn(&t_id,"maTache",0,99,T_JOINABLE,&f_task,NULL))!=0) {
    printf("ERREUR rt_task_spawn %d!\n",n);
    return 1;
  }

  rt_task_join(&t_id);
  return 0;
}

