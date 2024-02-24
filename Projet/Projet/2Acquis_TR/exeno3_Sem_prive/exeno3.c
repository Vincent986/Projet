/* exemple de mise a niveau de la tache main
   avec synchronisation par semaphore
************************************* */
#include <stdio.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/sem.h>

static RT_SEM sem_id;

static void f_task(void *arg)
{
volatile int i = 0;
int err;

  /* calcul */
  for (i=0;i<100000000;++i);

  if ((err=rt_sem_v(&sem_id))!=0) printf("ERREUR rt_sem_v %d\n",err);
}


int main (int N, char *P[])
{
RT_TASK task_id, task_main;
int err;

  mlockall(MCL_CURRENT | MCL_FUTURE);

  if (rt_sem_create(&sem_id, "monSem", 0, S_FIFO) != 0) {
    printf("ERREUR rt_sem_create !\n");
    return 1;
  }

  if ((err=rt_task_spawn(&task_id,"maTache",0,99,0,&f_task,NULL))!=0) {
    printf("ERREUR rt_task_spawn %d!\n",err);
    return 1;
  }

/* a decommenter pour la seconde compilation
************* */
  if (rt_task_shadow(&task_main, "monMain", 99, 0) != 0) {
    printf("ERREUR rt_task_shadow !\n");
    return 1;
  }

  /* attente de la fin de "maTache" (idem rt_task_join) */
  if ((err=rt_sem_p(&sem_id,TM_INFINITE))!=0) {
    printf("ERREUR rt_sem_p %d!\n",err);
    return 1;
  }
  printf("Fin du main !\n");
  return 0;
}

