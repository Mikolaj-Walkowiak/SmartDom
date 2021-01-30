#include "inside_lights.h"
#define PIN_COUNT 8
static const struct device *inside_lights;
uint8_t inside_lights_pos = 255;

int inside_lights_init(){
    //inside_lights = device_get_binding("UART_1"); doesnt exist ;(((
        inside_lights = device_get_binding("GPIOB");
    if (!inside_lights) {
		printk("Lights controller couldn't connect to device\n");
        return 1;
	}
    int result = 0;
    for (int i=0; i<PIN_COUNT; i++) {
        //int gpio_pin_configure(const struct device *port, gpio_pin_t pin, gpio_flags_t flags)
		result = gpio_pin_configure(inside_lights,i,GPIO_OUTPUT_ACTIVE);
		if (result) {
			printk("ERROR: Couldn't configure GPIOB pin %d", i);
			return 1;
		}
    }
    return 0;
}
int inside_lights_set(uint8_t pos){
        inside_lights_pos = pos;
        int result = 0;
        //int gpio_pin_set_raw(const struct device *port, gpio_pin_t pin, int value)
        for (int i=0; i<PIN_COUNT; i++) {
            int one =1;
            result = gpio_pin_set_raw(inside_lights, i, pos & (one << i));
        }
        return result;
    }
int inside_lights_read(){
    return inside_lights_pos;
}