#include "light_sensor.h"

// Address(unchangable) 0x40
//* Measure Relative Humidity, Hold Master Mode         0xE5
//  * Measure Relative Humidity, No Hold Master Mode      0xF5 
#define I2C_ADDR 0x40
#define I2C_MODE 0xe5

static const struct device *sensor;

int sensor_init()
{
	sensor = device_get_binding("I2C_1");
	if (!sensor) {
		printk("Light sensor couldn't connect to device\n");
        return 1;
	}
    return 0;
}

int sensor_read()
{
	int result = 0;
	uint8_t data[2];
    
    //int i2c_burst_read(const struct device *dev, uint16_t dev_addr, uint8_t start_addr, uint8_t *buf, uint32_t num_bytes)
	if (!i2c_burst_read(sensor, I2C_ADDR, I2C_MODE, data, sizeof(data))) {
        int humidity  = ((data[0] * 256) + data[1]);
        humidity = ((125 * humidity) / 65536) - 5;
        return humidity;
	} else {
        printk("Warning: I2C couldn't read from the light sensor\n");
		return -1;
	}
}