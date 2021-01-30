#include <zephyr.h>
#include <device.h>
#include <devicetree.h>

#include "light_sensor.h"
#include "blinds.h"
#include "inside_lights.h"
#include "interrupts.h"

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 350

int blinds_in_percent = 0;
uint8_t light_value = 0, blinds_value = 255;

void change_blinds_position(int light_level)
{
	int success = 0, success_l = 0;
	switch (light_level)
	{
	case 0 ... 25:
		blinds_value = 0;
		light_value = 255;
		success += blinds_set(blinds_value);
		success_l += inside_lights_set(light_value);
		blinds_in_percent = 100;
		break;
	case 26 ... 50:
		blinds_value = 64;
		light_value = 191;
		success += blinds_set(blinds_value);
		success_l += inside_lights_set(light_value);
		blinds_in_percent = 25;
		break;
	default:
		blinds_value = 255;
		light_value = 0;
		success += blinds_set(blinds_value);
		success_l += inside_lights_set(light_value);
		blinds_in_percent = 0;
		break;
	}
	printk("Moving the blinds to %d!\%\n", blinds_in_percent);
	if (success)
	{
		printk("Error while moving the blinds! Please try and restart the device!");
	}
	if (success_l)
	{
		printk("Error while changing the light level! Please try and restart the device!");
	}
}

void main(void)
{
	int setup_check = 0;
	/* SETUP THE SYSTEM */
	setup_check += sensor_init();
	setup_check += blinds_init();
	setup_check += inside_lights_init();
	setup_check += interrupts_init();

	/* END SETUP */
	if (setup_check)
	{
		printk("OOPSIE POOPSIE SOMETHING WENT WRONG\n");
		printk("SHUTTING DOWN\n");
		return;
	}
	printk("WELCOME, ALL SYSTEMS ARE OPERATIONAL\n");
	int light_level = 100, last_light_level = 100, after_reset = 1;

	while (1)
	{
		if (check_power())
		{
			printk("===================\n");
			if (check_alarm())
			{
				printk("IOIOIOIOioioioIOIOOIOIioioioIOIOIOIOIOIOIO!\n");
				printk("ALARM ACTIVATED! PRESS THE BUTTON TO DEACTIVATE!\n");
				after_reset = 1;
			}
			else
			{
				if (after_reset)
				{
					blinds_set(blinds_value);
					inside_lights_set(light_value);
					after_reset = 0;
				}
				light_level = sensor_read();
				printk("Current light level: %d\n", light_level);
				uint8_t current_pos = blinds_read();
				printk("Blinds are at %d!\% (position = %d)\n", blinds_in_percent, current_pos);
				printk("Lights are at %d!\%\n", blinds_in_percent);
				if (!(last_light_level == light_level))
				{
					change_blinds_position(light_level);
					last_light_level = light_level;
				}
			}
			printk("===================\n");
			k_msleep(SLEEP_TIME_MS);
		}
		else
		{
			after_reset = 1;
			//k_msleep(5*SLEEP_TIME_MS);
			k_msleep(5 * SLEEP_TIME_MS);
		}
	}
}
