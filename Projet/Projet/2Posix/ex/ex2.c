#include <stdio.h>
#include <unistd.h>

int a=100;

int main(int argc, char* argv[]) 
{
        if (fork()==0) a+=100;
        else	a+=200;
	sleep(1);
	printf("a = %d\n",a);
	return 0;
}
