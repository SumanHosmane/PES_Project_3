#include "dma.h"
#include "uart.h"
#include "gpio.h"
#include "app.h"

void DMA_config()
{
	// Enable clock for DMA and DMAMUX
	SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
	SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;

	// Enable Channel 0 and set Port D as DMA request source
	DMAMUX0->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(40);

	// Set memory address for source and destination
	DMA_SAR0 = ADC_reg_addr;							//Data register of ADC0 channel
	DMA_DAR0 = base_addr;								//Destination address set to
														//base address of allocated buffer

	DMA_DCR0 = 0x202C0000;								//setting CS, DSIZE, DINC and SSIZE bits

	DMA_DSR_BCR0 &= !(DMA_DSR_BCR_DONE_MASK);			//Making DONE bit 0
	DMA_DSR_BCR0 |= 0x80;								//80 = 128; because after each transfer
														//BCR decrements by 2 for 16 bit transfer

	DMA_DCR0 |= DMA_DCR_ERQ_MASK;						//Enabling peripheral trigger (PORT40 - ADC) for DMA

	__enable_irq();

	NVIC_EnableIRQ(DMA0_IRQn);
}

void DMA0_IRQHandler()
{
	char pretext[20];
	__disable_irq();

	GPIOD_PTOR |= 0x20;									//Toggling pin 5 (6th pin) of PORTD

	UART_transmit('\r');
	UART_transmit('\n');

	if(peak_flag!= 1)
	{
		peak = alpha * peak;							//changing the peak flag if new peak found
	}

	peak_flag = 0;

	sprintf(pretext, "Peak value is %d", peak);
	//sprintf(peak_buffer, "%d", peak);					//print peak on to the UART

	for(int i=0; i<20; i++)
	{
		UART_transmit(pretext[i]);
	}
	UART_transmit('\r');
	UART_transmit('\n');


	DMA_DAR0 = base_addr;								//Destination address reassigned to base address
	head = base_addr;
	DMA_DCR0 &= !(DMA_DCR_EINT_MASK);					//Disabling DMA interrupt bit
	return;
}