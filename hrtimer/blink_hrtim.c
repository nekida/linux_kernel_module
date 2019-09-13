#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/hrtimer.h>
#include <linux/gpio.h>
#include <linux/time64.h>

MODULE_LICENSE		("GPL");
MODULE_AUTHOR		("Nikita Shestakov");
MODULE_DESCRIPTION	("GPIO blink on hrtimer");
MODULE_VERSION		("0.1");

static struct hrtimer 	my_hrtimer;
static struct gpio	blink_pin;
static int	pin_number 	= 18;
static int	pin_level	= 0;

static int 	timer_period 	= 10;
static bool	use_msec	= true;
static int	temp_time	= 0;

module_param(timer_period, int, 0755);
module_param(use_msec, bool, 0755);

static enum hrtimer_restart my_hrtimer_callback(struct hrtimer * timer)
{
	pin_level = !pin_level;
	gpio_set_value(pin_number, pin_level);
	hrtimer_forward_now(&my_hrtimer, ktime_set(0, temp_time)); //next trigger timer

	return HRTIMER_RESTART;	//timer must be restarting 
}

static int __init module_blink_hrtim_init(void)
{
	temp_time = (use_msec) ? (timer_period * NSEC_PER_MSEC) : (timer_period * NSEC_PER_USEC);

	printk(KERN_INFO "Module is load\n");

	blink_pin.gpio 	= pin_number;
	blink_pin.flags	= GPIOF_OUT_INIT_LOW;
	blink_pin.label	= "PIN_18";
	if(!gpio_is_valid(pin_number))// pin valid?
	{
		printk(KERN_ALERT "Number pin %d gpio not valid!\n", pin_number);
		return -ENODEV;
	}
	if(gpio_request(pin_number, "sysfs"))	//request pin at system
	{
		printk(KERN_ALERT "Failed gpio request pin %d gpio!\n", pin_number);
	}
	gpio_direction_output(pin_number, 1);	// pin on exit (number pin, initial value)
	//gpio_export(pin_number, false);
	gpio_set_value(pin_number, pin_level);	//set value
	hrtimer_init(&my_hrtimer, CLOCK_REALTIME, HRTIMER_MODE_REL);//(timer, used clock, timer mode)
	my_hrtimer.function = my_hrtimer_callback;
	hrtimer_start(&my_hrtimer, ktime_set(0, temp_time), HRTIMER_MODE_REL); //ktime - set ktime_t from value sec/nanosec (sec, nanosec)

	return 0;
}

static void __exit module_blink_hrtim_exit(void)
{
	gpio_free(pin_number);
	hrtimer_cancel(&my_hrtimer);
	printk(KERN_INFO "Module unload\n");
}

module_init(module_blink_hrtim_init);
module_exit(module_blink_hrtim_exit);
