#include <zephyr.h>
#include <device.h>
#include <devicetree.h>


/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 350


void main(void)
{
	printk("WELCOME, ALL SYSTEMS ARE OPERATIONAL\n");
	while (1)
	{
		k_msleep(SLEEP_TIME_MS);
		printk("test\n");
	}
}
