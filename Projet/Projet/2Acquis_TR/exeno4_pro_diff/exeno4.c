/* utilisation des semaphores et des mutex
On peut utiliser /proc/xenomai/registry pour debugger
***************************************/
#include <stdio.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/sem.h>
#include <native/mutex.h>

static RT_SEM sem1, sem2; /* en tant que semaphores prives */
static RT_MUTEX mutex;
static int varCommune;

static void f_task1(void *arg)
{
int i,n;
  for(i=0;i<20;i++) {
    /* appel a la fct P() */
    if ((n=rt_sem_p(&sem1,TM_INFINITE))!=0) printf("ERREUR rt_sem_p %d\n",n);
    printf("Tache 1 au travail\n");
    /* appel a la fct V() */
    if ((n=rt_sem_v(&sem2))!=0) printf("ERREUR rt_sem_v %d\n",n);
  }
}

static void f_task2(void *arg)
{
int i,n;
  for(i=0;i<20;i++) {
    /* appel a la fct P() */
    if ((n=rt_sem_p(&sem2,TM_INFINITE))!=0) printf("ERREUR rt_sem_p %d\n",n);
    printf("Tache 2 au travail\n");
    /* appel a la fct V() */
    if ((n=rt_sem_v(&sem1))!=0) printf("ERREUR rt_sem_v %d\n",n);
  }
}

/* definition de la fct commune pour les taches 3 et 4 */
static void f_task(void *arg)
{
unsigned int nbT=10;
char *nomTache = (char*)arg;

  while(nbT--) {
    rt_task_yield(); /* pour faciliter l'ordonnancement */
    /* on fait P() sur le mutex */
    if (rt_mutex_acquire(&mutex, TM_INFINITE) != 0)
      printf("ERREUR rt_mutex_acquire !\n");
    varCommune++;
    printf("%s, varCommune=%d\n",nomTache,varCommune);

    /* wait pour forcer la distribution des taches Round-Robin */
    rt_task_sleep(1000000);

    /* on fait V() sur le mutex */
    if (rt_mutex_release(&mutex) != 0)
      printf("ERREUR rt_mutex_release !\n");
  }
}

int main (void)
{
RT_TASK task_id1,task_id2,task_id3,task_id4;
int err=0;

  mlockall(MCL_CURRENT | MCL_FUTURE);

  /* la 1ere tache demarre et la seconde est bloquee !! */
  if (rt_sem_create(&sem1, "monSem1", 1, S_FIFO) != 0) {
    printf("ERREUR rt_sem_create !\n");
    return 1;
  }
  if (rt_sem_create(&sem2, "monSem2", 0, S_FIFO) != 0) {
    printf("ERREUR rt_sem_create !\n");
    return 1;
  }
  /* initialisation du mutex */
  if (rt_mutex_create(&mutex, "monMutex") != 0) {
    printf("ERREUR rt_mutex_create !\n");
    return 1;
  }

  if ((err=rt_task_spawn(&task_id1,"maTache 1",0,99,T_JOINABLE,&f_task1,
      NULL))!=0) {
    printf("ERREUR rt_task_spawn %d\n",err);
    return 1;
  }
  if ((err=rt_task_spawn(&task_id2,"maTache 2",0,99,T_JOINABLE,&f_task2,
      NULL))!=0) {
    printf("ERREUR rt_task_spawn %d\n",err);
    return 1;
  }

  /* definition des taches 3 et 4 en plus basse priorite */
  if ((err=rt_task_spawn(&task_id3,"maTache 3",0,50,T_JOINABLE,&f_task,
      "maTache 3"))!=0) {
    printf("ERREUR rt_task_spawn %d\n",err);
    return 1;
  }
  if ((err=rt_task_spawn(&task_id4,"maTache 4",0,50,T_JOINABLE,&f_task,
      "maTache 4"))!=0) {
    printf("ERREUR rt_task_spawn %d\n",err);
    return 1;
  }

  /* attentes des taches */
  rt_task_join(&task_id1);
  rt_task_join(&task_id2);
  rt_task_join(&task_id3);
  rt_task_join(&task_id4);

  return 0;
}


