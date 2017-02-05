#include <linux/list.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
/* This function is called when the module is loaded. */

struct birthday{
	int day;
	int month;
	int year;
	struct list_head list;
};

	static LIST_HEAD(birthday_list);


int simple_init(void)
{
        struct birthday *person;
	int i = 0;
        printk(KERN_INFO "Loading Module\n");  
	for( i=4 ; i>=0 ; i--){
	person = kmalloc(sizeof(*person), GFP_KERNEL);
	person->day = 2+i;
	person->month = 8+i;
	person->year = 1995+i;
	INIT_LIST_HEAD(&person->list);
	list_add_tail(&person->list, &birthday_list);
	}

	list_for_each_entry(person, &birthday_list, list){
        printk("year:%d,month%d,day:%d\n",person->year,person->month, person->day);
        }
	return 0;	
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	struct birthday  *cur ,*next;
	printk(KERN_INFO "Removing Module\n");
	list_for_each_entry_safe(cur,next,&birthday_list,list) {
	list_del(&cur->list);
	kfree(cur);
	}
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
