#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <native/task.h>
#include <rtdk.h>

#define DELAY_NS	1000000000

// empty signal handler
void sigint_handler(int sig)
{
}

// task function implementation
void aperiodic_task(void *param)
{
	RT_TASK_INFO info;
	int i;
	// get info about current task
	rt_task_inquire(NULL, &info);
	for(i=1; i<10; i++)
	{
		rt_printf("%s : %d (%d)\n", info.name, i, info.cprio);
	}
	// destroy current task -- NULL
	rt_task_delete(NULL);
}

int main()
{
	RT_TASK tasks[5];
	int ret, i, prio;
	struct sigaction sa;
	char name[8];

	// all pages of current process become logical
	// i.e. they will not be swapped out/in
	// ensures no delay in execution of task due to paging activity.
	ret = mlockall(MCL_CURRENT | MCL_FUTURE);

	// registering SIGINT signal handler
	// ensure that process is not terminated due to ctrl+C
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = sigint_handler;
	ret = sigaction(SIGINT, &sa, NULL);

	// create a background task that flush rt_printf's buffer periodically
	// on current terminal -- period is 1 ms (arg).
	rt_print_auto_init(1);
	rt_printf("main() started.\n");
	
	prio = 10;
	for(i=0; i<5; i++)
	{
		// create a task
		sprintf(name, "Task%d", i);
		ret = rt_task_create(&tasks[i], name, 0, prio, T_CPU(0)); 
		if(ret != 0)
		{
			rt_printf("rt_task_create() failed.\n");
			_exit(1);
		}
		prio = prio + 20;
	}

	for(i=0; i<5; i++)
	{
		// begin execution of task
		ret = rt_task_start(&tasks[i], aperiodic_task, NULL);
		if(ret != 0)
		{
			rt_printf("rt_task_start() failed.\n");
			_exit(1);
		}
	}

	// wait for any signal to arrive
	// then signal handler is executed and
	// execution progresses further
	pause();

	rt_printf("main() completed.\n");
	return 0;
}

