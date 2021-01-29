#include <zephyr.h>
#include <device.h>
#include <devicetree.h>

#include "light_sensor.h"

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 350


void main(void)
{
	int setup_check = 0;
	/* SETUP THE SYSTEM */
	setup_check += sensor_start();

	/* END SETUP */
	if(setup_check){
		printk("OOPSIE POOPSIE SOMETHING WENT WRONG\n");
		printk("SHUTTING DOWN\n");
		return;
	}
	printk("WELCOME, ALL SYSTEMS ARE OPERATIONAL\n");
	while (1)
	{
		k_msleep(SLEEP_TIME_MS);
		printk("Current light level: %d\n",sensor_read());
	}
}
