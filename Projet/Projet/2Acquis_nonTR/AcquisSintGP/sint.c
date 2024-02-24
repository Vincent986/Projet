/* sint.c
 * exemple de fabrication d'un fichier pour gnuplot
 * pour compiler : cc -o sint sint.c -lm
 * *************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define NBP 629 /* 2 x Pi = 6,28 !! */

int fid;

void ecritfic(char * b)
{
   write(fid,b,strlen(b));
}

int main (int ac, char **av)
{
int i;
char buf[100];

   if ((fid = creat("sinus.txt",0644)) == -1) {
      perror("Creation sinus.txt");
      return 1;
   }
   for(i=0;i<NBP;i++) {
      sprintf(buf,"%d %g\n",i,sin((double)i*0.01));
      ecritfic(buf);
   }
   ecritfic("\n");
   close(fid);
   return 0;
}

