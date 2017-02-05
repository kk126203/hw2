#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kernel.h>
/* This function is called when the module is loaded. */

struct birthday{
	int day;
	int month;
	int year;
	struct list_head list;
};

	static LIST_HEAD(birthday_list);

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

static inline void a_list_add_tail(struct list_head *new, struct list_head *head)
 {
      __list_add(new, head->prev, head);
 }

#ifndef CONFIG_DEBUG_LIST
static inline void __alist_del_entry(struct list_head *entry)
 {
         __list_del(entry->prev, entry->next);
 }
 
static inline void a_list_del(struct list_head *entry)
 {
         __list_del(entry->prev, entry->next);
         entry->next = LIST_POISON1;
         entry->prev = LIST_POISON2;
 }
#else
extern void __list_del_entry(struct list_head *entry);
extern void list_del(struct list_head *entry);
#endif


#define a_list_for_each_entry_safe(pos, n, head, member)                  \
         for (pos = list_first_entry(head, typeof(*pos), member),        \
                 n = list_next_entry(pos, member);                       \
              &pos->member != (head);                                    \
              pos = n, n = list_next_entry(n, member))


#define a_list_for_each_entry(pos, head, member)                          \
         for (pos = list_first_entry(head, typeof(*pos), member);        \
              &pos->member != (head);                                    \
              pos = list_next_entry(pos, member))



int simple_init(void)
{
        
        struct birthday *person;
	int i = 0;
	
        printk(KERN_INFO "Loading Module\n");
	for( i=0 ; i<5 ; i++){
	person = kmalloc(sizeof(*person), GFP_KERNEL);
	person->day = 2+i;
	person->month = 8+i;
	person->year = 1995+i;
	INIT_LIST_HEAD(&person->list);
	a_list_add_tail(&person->list, &birthday_list);
	}

	a_list_for_each_entry(person, &birthday_list, list){
        printk("year:%d,month%d,day:%d\n",person->year,person->month, person->day);
        }
	return 0;	
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	struct birthday  *cur ,*next;
	printk(KERN_INFO "Removing Module\n");
	a_list_for_each_entry_safe(cur,next,&birthday_list,list) {
	a_list_del(&cur->list);
	kfree(cur);
	}
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
