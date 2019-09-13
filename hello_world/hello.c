#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nikita Shestakov");
MODULE_DESCRIPTION("A simple Linux LKM for the RPi.");
MODULE_VERSION("0.1");

static char *name = "Hello world";

module_param(name, charp, S_IRUGO);
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log");

static int __init helloERPi_init(void) 
{
	printk(KERN_INFO "Module is load: %s!\n", name);
	return 0;
}

static void __exit helloERPi_exit(void) 
{
	printk(KERN_INFO "Goodbye!\n");
}

module_init(helloERPi_init);
module_exit(helloERPi_exit);
