#include <stdio.h>
#include <drivers/gpio.h>
#include <drivers/uart.h>

int blinds_init();
int blinds_set(uint8_t pos);
int blinds_read();