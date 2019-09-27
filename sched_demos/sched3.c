#include <stdio.h>
#include <unistd.h>

int a, b;
volatile int c;
// round-robin
int main()
{
	int ret, i;
	ret = fork();
	if(ret == 0)
	{
		for(i=1; i<=10000; i++)
		{
			printf("P1 : %d\n", i);
			for(a=0; a<1000; a++)
				for(b=0; b<1000; b++)
					c++;
		}
	}
	else
	{
		for(i=1; i<=10000; i++)
		{
			printf("P2 : %d\n", i);
			for(a=0; a<1000; a++)
				for(b=0; b<1000; b++)
					c++;
		}
	}
	return 0;
}

// taskset 0x00000001 chrt -r 50 ./sched1


