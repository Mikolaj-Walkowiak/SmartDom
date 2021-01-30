#include "interrupts.h"
static const struct device *irq_handler;
static struct gpio_callback irq_data;

bool is_on = true;
bool is_fire = false;
int is_init = 0;
//typedef void (*gpio_callback_handler_t)(const struct device *port, struct gpio_callback *cb, gpio_port_pins_t pins)
void handler(const struct device *dev, struct gpio_callback *data, gpio_port_pins_t pins)
{
        if (pins & 0x1)
    {
        is_on = !is_on;
        if (is_on)
        {
            printk("Welcome back!\n");
        }
        else
        {
            printk("Shhhh, the uC is sleeping, please be quiet!\n");
            printk("ZZzzZZz\n");
        }
    }
    if (pins & 0x2)
    {
        is_fire = true;
        printk("FIRE DETECTED!\n");
        gpio_pin_set_raw(irq_handler, 3, 1);
    }
    if (pins & 0x4)
    {
        is_fire = false;
        printk("Alarm cleared! We hope everything is OK!\n");
        gpio_pin_set_raw(irq_handler, 3, 0);
    }

}

bool check_power()
{
    return is_on;
}
bool check_alarm()
{
    return is_fire;
}

int interrupts_init()
{
    irq_handler = device_get_binding("GPIOA");
    if (!irq_handler)
    {
        printk("IRQ controller couldn't connect to device\n");
        return 1;
    }
    int result = 0;
    for (int i = 0; i < 4; ++i)
    {
        result += gpio_pin_configure(irq_handler, i, GPIO_INPUT);
        if (result)
        {
            printk("ERROR: Couldn't configure GPIOC pin %d\n", i);
        }
        //int gpio_pin_interrupt_configure(const struct device *port, gpio_pin_t pin, gpio_flags_t flags)
        result += gpio_pin_interrupt_configure(irq_handler, i, GPIO_INT_EDGE_TO_ACTIVE);
        if (result)
        {
            printk("ERROR: Couldn't configure IRQ on pin %d\n", i);
        }
    }
    result += gpio_pin_configure(irq_handler, 3, GPIO_OUTPUT_INACTIVE);
    if (result)
    {
        printk("ERROR: Couldn't configure GPIOC pin %d\n", 3);
    }

    //void gpio_init_callback(struct gpio_callback *callback, gpio_callback_handler_t handler, gpio_port_pins_t pin_mask)
    uint8_t pins = 7; // eq to 3 last pins binary(111)
    gpio_init_callback(&irq_data, handler, pins);
    gpio_add_callback(irq_handler, &irq_data);

    return result;
}
