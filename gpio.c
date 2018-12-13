#include "gpio.h"

void gpio_config()
{
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;					//configuring PORTD clock for GPIO
	PORTD_PCR5 |= 0x100;								//Setting the 6th pin (PIN 5) in PORTD

	GPIOD_PDDR |= 0x20;									//configuring GPIO pin 5 (6th pin) of port D
	GPIOD_PSOR |= 0x20;									//enabling set and reset of PORTD-5 GPIO
}