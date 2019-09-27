#include <stdio.h>
#include <unistd.h>

int main()
{
	int ret, i;
	ret = fork();
	if(ret == 0)
	{
		for(i=0; i<100; i++)
		{
			printf("P1 : %d\n", i);
			sleep(1);
		}
	}
	else
	{
		for(i=0; i<100; i++)
		{
			printf("P2 : %d\n", i);
			sleep(1);
		}
	}
	return 0;
}

// ./sched
// nice -n +4 ./sched

// ps -e -o pid,cls,ni,cmd


