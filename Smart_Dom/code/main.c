#include <zephyr.h>
#include <device.h>
#include <devicetree.h>

#include "light_sensor.h"
#include "blinds.h"

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 350

int blinds_in_percent = 0;


void change_blinds_position(int light_level){
	int success = 0;
switch (light_level)
			{
			case 0 ... 25:
				success+= blinds_set(0);
				blinds_in_percent = 100;
				break;
			case 26 ... 50:
				success+= blinds_set(64);
				blinds_in_percent = 25;
				break;
			default:
				success+= blinds_set(255);
				blinds_in_percent = 0;
				break;
			}
			printk("Moving the blinds to %d!\%\n",blinds_in_percent);
			if(success){
				printk("Error while moving the blinds! Please try and restart the device!");
			}
}



void main(void)
{
	int setup_check = 0;
	/* SETUP THE SYSTEM */
	setup_check += sensor_init();
	setup_check += blinds_init();

	/* END SETUP */
	if (setup_check)
	{
		printk("OOPSIE POOPSIE SOMETHING WENT WRONG\n");
		printk("SHUTTING DOWN\n");
		return;
	}
	printk("WELCOME, ALL SYSTEMS ARE OPERATIONAL\n");
	int light_level = 100, last_light_level = 100;
	blinds_set(255);
	while (1)
	{
		light_level = sensor_read();
		printk("Current light level: %d\n", light_level);
		uint8_t current_pos = blinds_read();
		printk("Blinds are at %d!\% (position = %d)\n",blinds_in_percent,current_pos);
		if (!(last_light_level == light_level))
		{
			change_blinds_position(light_level);
			last_light_level = light_level;
		}
		k_msleep(SLEEP_TIME_MS);
	}
}
