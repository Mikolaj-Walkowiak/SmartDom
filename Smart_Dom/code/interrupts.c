#include "interrupts.h"
static const struct device *irq_handler;
static struct gpio_callback irq_data;

bool is_on = true;
bool is_fire = false;


int interrupts_init()
{
	irq_handler = device_get_binding("GPIOC");
	if (!irq_handler) {
		printk("IRQ controller couldn't connect to device\n");
		return 1;
	}
	int result = 0;
    for(int i = 0; i<4;++i){
        result += gpio_pin_configure(irq_handler, i, GPIO_INPUT);
        if(result){printk("ERROR: Couldn't configure GPIOC pin %d\n", i);}
        //int gpio_pin_interrupt_configure(const struct device *port, gpio_pin_t pin, gpio_flags_t flags)
        result += gpio_pin_interrupt_configure(irq_handler, i, GPIO_INT_EDGE_TO_ACTIVE);
        if(result){printk("ERROR: Couldn't configure IRQ on pin %d\n", i);}
    }
    result += gpio_pin_configure(irq_handler, 4, GPIO_OUTPUT_INACTIVE);
    if(result){printk("ERROR: Couldn't configure GPIOC pin %d\n", 4);}

	return result;
}
