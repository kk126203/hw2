#include <linux/list.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/sched.h>
/* This function is called when the module is loaded. */

void dfs(struct task_struct *task,int i)
{
	struct task_struct *cur;
	struct list_head *list;
	char pri[100]={"\0"};
	char str[15];
	char buf[50];
	pid_t pid = task->pid;
	long state = task->state;
	int j;
	for( j=0 ; j<i ; j++)
	{
	      strcat(pri,"    ");
	}	
	strcat(pri,"  Name:");
	strcat(pri,task->comm);
	strcat(pri,"    pid :");
	sprintf(str,"%d",pid);
	sprintf(buf,"%lu",state);
	strcat(pri,str);
	strcat(pri,"    state:");
	strcat(pri,buf);
	printk("%s\n",pri);
	i+=1;
	list_for_each(list, &task->children){
		cur = list_entry(list,struct task_struct,sibling);
		dfs(cur,i);
		}
}

int simple_init(void)
{
	int i = 0;
 	printk(KERN_INFO "Loading Module\n");
	dfs(&init_task,i);
	return 0;	
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n");
	}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
