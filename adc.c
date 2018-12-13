#include <MKL25Z4.h>
#include <stdio.h>
#include <stdint.h>
#include "adc.h"

void ADC_config()
{
	//enable clock for adc0
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;					//enabling ADC clock
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;					//enabling PORTE clock

	PORTA_PCR20 = PORT_PCR_MUX(0);
	PORTA_PCR21 = PORT_PCR_MUX(0);

	ADC0_CFG1 = 0; // Reset register
	ADC0_CFG1 |= (ADC_CFG1_MODE(3) | ADC_CFG1_ADICLK(0) | ADC_CFG1_ADIV(1));	//selecting 48/8 = 6MHz clock
																				//Busclk = busclk/2
																				//DIFF = 1, 16 bit mode

	ADC0_SC1A = 0x60;									//DIFF = 1, Interrupt enabled

	ADC0_SC2 |= ADC_SC2_DMAEN_MASK;    					// DMA Enable
	ADC0_SC3 = 0x0F;									//Continuous Conversion, Avg, Hardware avg select
}
