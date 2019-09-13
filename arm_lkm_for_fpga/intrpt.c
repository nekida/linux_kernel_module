#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nikita Shestakov");
MODULE_DESCRIPTION("Module messages of end proccessing fpga");
MODULE_VERSION("0.1");

static int pin_number = 18;
static int irq_number;
static int number_presses = 0;

static irq_handler_t interrupt_handler(unsigned int irq, void * dev_id, struct pt_regs * regs)
{
	number_presses++;
	printk(KERN_INFO "Counter press is %d\n", number_presses);

	return (irq_handler_t)IRQ_HANDLED;
}

static int __init load_module(void)
{
	int result = 0;

	printk(KERN_INFO "Module is load\n");
	if(gpio_request(pin_number, "sysfs"))
		printk(KERN_ALERT "Failed gpio request pin %d gpio!\n", pin_number);
	gpio_direction_input(pin_number);
  	gpio_set_debounce(pin_number, 200);
	gpio_export(pin_number, false);
	printk(KERN_INFO "GPIO test: level on pin is: %d\n", gpio_get_value(pin_number));

	irq_number = gpio_to_irq(pin_number);
	printk(KERN_INFO "GPIO test: number IRQ = %d\n", irq_number);

	result = request_irq(irq_number, (irq_handler_t)interrupt_handler, IRQF_TRIGGER_FALLING, "gpio_handler", NULL);
	printk(KERN_INFO "GPIO test: IRQ result is %d\n", result);

  	return result;
}

static void __exit unload_module(void)
{
	free_irq(irq_number, NULL);
	gpio_unexport(pin_number);
	gpio_free(pin_number);
	printk(KERN_INFO "Module is unload\n");
}

module_init(load_module);
module_exit(unload_module);


