#include <unistd.h>
#include <stdio.h>

#if _POSIX_VERSION < 199309L 
#error POSIX4 absent
#else
#ifndef _POSIX_PRIORITY_SCHEDULING
#error POSIX4 pas d''ordonnancement HPF
#endif
#endif

int main(int N, char *P[])
{
   printf("Le nombre de tops d'horloge par seconde est %ld\n",
		sysconf(_SC_CLK_TCK));
   return 0;
}

