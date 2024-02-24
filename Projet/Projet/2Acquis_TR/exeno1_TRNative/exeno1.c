/* premier exemple simple : pour tester la compilation */
#include <stdio.h>
#include <sys/mman.h>
#include <native/task.h>


static void fct_task(void *arg)
{
int i, j;

   /* par defaut en primary mode */
   j=0;
   for (i=0;i<1000000;i++) j++;
   /* Jusqu'a ce que l'on fasse appel a une fct de l'environnement Linux */
   printf("Bonjour a tous !\n");
   /* Ici printf a besoin des ressources de Linux donc
      nous sommes maintenant en secondary mode */
   return;
}

int main (int N, char *P[])
{
/* exemple d'utilisation de rt_rask_spawn()
****** */
RT_TASK task_id;

   /* on empeche le swap */
   mlockall(MCL_CURRENT | MCL_FUTURE);

   if (rt_task_spawn(&task_id,  /* descripteur de la tache */
        "maTache",  /* nom */
        0           /* 0 = default stack size */,
        99          /* priorite */,
        T_JOINABLE, /* car on va appeler rt_task_join */
        &fct_task,  /* point d'entree (adresse de la fonction) */
        NULL        /* argument */)    !=0) {
      printf("ERREUR rt_task_spawn !\n");
      return 1;
   }
   /* attente de la fin de la tache */
   rt_task_join(&task_id);
   return 0;
}


