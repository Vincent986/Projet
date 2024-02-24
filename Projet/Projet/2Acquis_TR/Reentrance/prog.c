#include <signal.h>

int A, b=50, C=25, res=12, Var1, I=7;

int toto(int a, int b)
{
int res; /* ces lignes correspondent a l'intruction return (a+b); */
   res=a;
        /*    <= Interruption !! */
   res+=b;
   return res;
}

void interrupt(int S)
{
int t,u,v;
   t=-78;
   u=17;
   v=toto(t,u);
}

int main (int N, char **P)
{
int x,y,z;

   signal(SIGINT,interrupt);
   x=12;
   y=56;
   z=toto(x,y);

   return 0;
}

