#include <stdio.h>
#include <unistd.h>

// manual round-robin
int main()
{
	int ret, i;
	ret = fork();
	if(ret == 0)
	{
		for(i=1; i<=20; i++)
		{
			printf("P1 : %d\n", i);
			if(i % 5 == 0)
				sched_yield();
		}
	}
	else
	{
		for(i=1; i<=20; i++)
		{
			printf("P2 : %d\n", i);
			if(i % 5 == 0)
				sched_yield();
		}
	}
	return 0;
}

// taskset 0x00000001 chrt -f 50 ./sched1


