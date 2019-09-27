#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <native/task.h>

#define rt_printf printk

#define DELAY_NS	1000000000
static RT_TASK t1;

//step1: implement task function
void task_func(void *param)
{
	int count = 1;
	char *msg = (char*)param;
	rt_task_set_periodic(NULL, TM_NOW, DELAY_NS);
	while(1)
	{
		rt_printf("%s -- %d\n", msg, count);
		count++;
		rt_task_wait_period(NULL);
	}
}

static int __init rttask_init(void)
{
	int ret;
	rt_printf("rttask_init() called.\n");
	// step2: create the task
	ret = rt_task_create(&t1, "T1", 0, 50, T_CPU(0));
	if(ret != 0)
		return -1;
	// step3: start the task
	ret = rt_task_start(&t1, task_func, "Counting Task");
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
}

module_init(rttask_init);
module_exit(rttask_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Desd Sunbeam <desd@sunbeaminfo.com>");
MODULE_DESCRIPTION("Periodic Xenomai RT tasks demo.");


