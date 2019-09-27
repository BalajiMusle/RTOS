#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/sem.h>
#include <native/mutex.h>
#include <rtdk.h>

#define DELAY_NS	1000000000

RT_SEM s;
RT_MUTEX m;
int count = 0;

// empty signal handler
void sigint_handler(int sig)
{
}

// task function implementation -- increment
void aperiodic_task1(void *param)
{
	RT_TASK_INFO info;
	int i;
	// *** do p operation on semaphore -- pause the task
	rt_sem_p(&s, TM_INFINITE);

	// get info about current task
	rt_task_inquire(NULL, &info);
	for(i=1; i<=10; i++)
	{
		// lock mutex
		rt_mutex_acquire(&m, TM_INFINITE);
		count++;
		rt_printf("%s : count = %d\n", info.name, count);
		// unlock mutex
		rt_mutex_release(&m);
	}
	// destroy current task -- NULL
	rt_task_delete(NULL);
}

// task function implementation -- decrement
void aperiodic_task2(void *param)
{
	RT_TASK_INFO info;
	int i;
	// *** do p operation on semaphore -- pause the task
	rt_sem_p(&s, TM_INFINITE);

	// get info about current task
	rt_task_inquire(NULL, &info);
	for(i=1; i<=10; i++)
	{
		// lock mutex
		rt_mutex_acquire(&m, TM_INFINITE);
		count--;
		rt_printf("%s : count = %d\n", info.name, count);
		// unlock mutex
		rt_mutex_release(&m);
	}
	// destroy current task -- NULL
	rt_task_delete(NULL);
}

int main()
{
	RT_TASK task1, task2;
	int ret, i, prio;
	struct sigaction sa;

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

	// *** init sem count = 0
	ret = rt_sem_create(&s, "SEM1", 0, S_FIFO);

	// init mutex
	ret = rt_mutex_create(&m, "MUT1");

	// create a task
	ret = rt_task_create(&task1, "Task1", 0, 50, T_CPU(0)); 
	if(ret != 0)
	{
		rt_printf("rt_task_create() failed.\n");
		_exit(1);
	}

	// begin execution of task
	ret = rt_task_start(&task1, aperiodic_task1, NULL);
	if(ret != 0)
	{
		rt_printf("rt_task_start() failed.\n");
		_exit(1);
	}

	// create a task
	ret = rt_task_create(&task2, "Task2", 0, 50, T_CPU(0)); 
	if(ret != 0)
	{
		rt_printf("rt_task_create() failed.\n");
		_exit(1);
	}

	// begin execution of task
	ret = rt_task_start(&task2, aperiodic_task2, NULL);
	if(ret != 0)
	{
		rt_printf("rt_task_start() failed.\n");
		_exit(1);
	}

	rt_printf("main(): press ctrl+c to resume all tasks.\n");
	pause();

	// *** perform semaphore broadcast -- resume all tasks -- at once
	rt_sem_broadcast(&s);

	rt_printf("main(): press ctrl+c to terminate.\n");
	pause();

	// destroy mutex
	rt_mutex_delete(&m);

	// *** destroy the semaphore
	rt_sem_delete(&s);

	rt_printf("main() completed : final count = %d.\n", count);
	return 0;
}

