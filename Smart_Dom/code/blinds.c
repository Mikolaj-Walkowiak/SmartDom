#include "blinds.h"
#define PIN_COUNT 4
#define PIN_SHIFT 4 // THANK YOU ZEPHYR, VERY COOL XDDDDDDDDDD

static const struct device *blinds;
uint8_t blinds_pos = 255;

int blinds_init(){
    //blinds = device_get_binding("UART_1"); doesnt exist ;(((
        blinds = device_get_binding("GPIOA");
    if (!blinds) {
		printk("Blinds controller couldn't connect to device\n");
        return 1;
	}
    int result = 0;
    for (int i=0; i<PIN_COUNT; i++) {
        //int gpio_pin_configure(const struct device *port, gpio_pin_t pin, gpio_flags_t flags)
		result = gpio_pin_configure(blinds,i + PIN_SHIFT,GPIO_OUTPUT_ACTIVE);
		if (result) {
			printk("ERROR: Couldn't configure GPIOA pin %d", i);
			return 1;
		}
    }
    return 0;
}
int blinds_set(uint8_t pos){
        blinds_pos = pos;
        int result = 0;
        //int gpio_pin_set_raw(const struct device *port, gpio_pin_t pin, int value)
        for (int i=0; i<PIN_COUNT; i++) {
            int one =1;
            result += gpio_pin_set(blinds, i + PIN_SHIFT, pos & (one << i));
        }
        return result;
    }
int blinds_read(){
    return blinds_pos;
}
