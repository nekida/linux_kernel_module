#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
 
static int __init pin_init(void)
{
  gpio_direction_output(18,1);
  while(1)
  {
	gpio_set_value(18, 1);
	gpio_set_value(18, 0);
  }
  printk(KERN_INFO "Pin is up!\n");
  return 0;
}
 
static void __exit pin_exit(void)
{
  gpio_set_value(18, 0);
  gpio_free(18);
  printk(KERN_INFO "Pin is down!\n");
}
 
module_init(pin_init);
module_exit(pin_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nikita Shestakov");
MODULE_DESCRIPTION("A simple LKM pin up - pin down");
MODULE_VERSION("0.1");
