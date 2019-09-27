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

// ./sched1
// taskset 0x00000001 ./sched1
// taskset 0x00000001 chrt -f 50 ./sched1
// taskset 0x00000001 chrt -r 50 ./sched1

// ps -e -o pid,cls,rtprio,cmd


