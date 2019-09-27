#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <native/task.h>
#include <native/pipe.h>

#define rt_printf printk

#define DELAY_NS	1000000000
static RT_TASK t1;
static RT_PIPE p1;

//step1: implement task function
void task_func(void *param)
{
	int i, num = 1, ret;
	char *msg = (char*)param;
	rt_task_set_periodic(NULL, TM_NOW, DELAY_NS);
	for(i=1; i<=100; i++)
	{
		ret = rt_pipe_write(&p1, &num, sizeof(num), P_NORMAL);
		rt_printf("%s -- %d\n", msg, num);
		num++;
		rt_task_wait_period(NULL);
	}
}

static int __init rttask_init(void)
{
	int ret;
	rt_printf("rttask_init() called.\n");
	ret = rt_pipe_create(&p1, "sensor_data", P_MINOR_AUTO, 1000);
	if(ret != 0)
		return -1;
	// step2: create the task
	ret = rt_task_create(&t1, "T1", 0, 50, T_CPU(0));
	if(ret != 0)
		return -1;
	// step3: start the task
	ret = rt_task_start(&t1, task_func, "DataAcqTask");
	if(ret != 0)
	{
		rt_task_delete(&t1);
		return -1;
	}
	return 0;
}

static void __exit rttask_exit(void)
{
	rt_printf("rttask_exit() called.\n");
	// destroy the task
	rt_task_delete(&t1);

	rt_pipe_delete(&p1);
}

module_init(rttask_init);
module_exit(rttask_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Desd Sunbeam <desd@sunbeaminfo.com>");
MODULE_DESCRIPTION("Periodic Xenomai RT tasks demo.");


