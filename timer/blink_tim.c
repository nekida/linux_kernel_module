#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/time64.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nikita Shestakov");
MODULE_DESCRIPTION("A pin blink simple timer in seconds");
MODULE_VERSION("0.1");

#define PIN     18

static struct timer_list my_timer;
static int pin_level = 0;
static int timer_period_in_sec = 0;

module_param(timer_period_in_sec, int, 0);

static void timer_callback(struct timer_list * timer)
{
  pin_level = !pin_level;
  gpio_set_value(PIN, pin_level);
  if(mod_timer(&my_timer, jiffies + msecs_to_jiffies \
				(timer_period_in_sec * MSEC_PER_SEC)))
    printk(KERN_ALERT "Error in callback timer_mod\n");
}

static int __init module_pin_timer_init(void)
{
  gpio_direction_output(PIN, 1);
  gpio_set_value(PIN, 1);
  timer_setup(&my_timer, timer_callback, 0);
  if(mod_timer(&my_timer, jiffies + msecs_to_jiffies(timer_period_in_sec * MSEC_PER_SEC)))
    printk(KERN_ALERT "Error in mod_timer!\n");
  printk(KERN_INFO "Module is loaded!\n");
  return 0;
}

static void __exit module_pin_timer_exit(void)
{
  gpio_set_value(PIN, 0);
  gpio_free(PIN);
  if(del_timer(&my_timer))
    printk(KERN_ALERT "Error on deleted timer!\n");
  printk(KERN_INFO "Module is unloaded!\n");
}

module_init(module_pin_timer_init);
module_exit(module_pin_timer_exit);

