/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 */


///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////
// SDK Included Files
#include "board.h"
#include "fsl_lptmr_driver.h"
#include "fsl_debug_console.h"

//--------------------our addition starts
#include <MKL25Z4.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "uart.h"
#include "adc.h"
#include "dma.h"
#include "gpio.h"
#include "app.h"

extern int16_t* base_addr;
extern int16_t* head;
extern int16_t adc_value;

//--------------------our addition ends

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
// Timer period: 500000uS
#define TMR_PERIOD         500000U
#if defined(TWR_KV46F150M)
#define LPTMR0_IDX LPTMR_IDX
#endif


////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief LPTMR interrupt call back function.
 * The function is used to toggle LED1.
 */
void lptmr_call_back(void)
{
    // Toggle LED1
    LED1_TOGGLE;
}

/*!
 * @brief Main function
 */
int main (void)
{
    // RX buffers
    //! @param receiveBuff Buffer used to hold received data

    // LPTMR configurations
 	lptmr_user_config_t lptmrConfig =
    {
        .timerMode = kLptmrTimerModeTimeCounter,
        .freeRunningEnable = false,
        .prescalerEnable = true,
        .prescalerClockSource = kClockLptmrSrcLpoClk,
        .prescalerValue = kLptmrPrescalerDivide2,
        .isInterruptEnabled = true,
    };
    // LPTMR driver state information
    lptmr_state_t lptmrState;

    // Initialize standard SDK demo application pins
    hardware_init();

    // Initialize LPTMR
    LPTMR_DRV_Init(LPTMR0_IDX, &lptmrState, &lptmrConfig);
    // Set timer period for TMR_PERIOD seconds
    LPTMR_DRV_SetTimerPeriodUs(LPTMR0_IDX, TMR_PERIOD);
    // Install interrupt call back function for LPTMR
    LPTMR_DRV_InstallCallback(LPTMR0_IDX, lptmr_call_back);
    // Start LPTMR
    LPTMR_DRV_Start(LPTMR0_IDX);

    // Initialize LED1
    LED1_EN;

//############################################################## our code starts here ###########################################################################

    UART_config();													//UART configuration
	ADC_config();

	char buffer[7];
	int valueDBFS;

	base_addr = (int16_t*) malloc(128);
	head = base_addr;

	DMA_config();
	gpio_config();

	while(1)
	{
		ADC0_SC1A = (0 & ADC_SC1_ADCH_MASK) | (ADC0_SC1A & ADC_SC1_DIFF_MASK);

		adc_value = ADC0_RA;

		valueDBFS = (fabs(adc_value))/32768;					//calculating dBFS
		valueDBFS = log10(valueDBFS);							//value of sample
		valueDBFS = 20*valueDBFS;

		sprintf(buffer, "%d", adc_value);						//printing ADC
		for(int i=0; i<6; i++)									//value on UART
		{
			UART_transmit(buffer[i]);
		}
		UART_transmit('\t');


		//calculating peak value
		app();

/*		sprintf(buffer, "%d", valueDBFS);			//printing dBFS value on UART
		for(int i=0; i<6; i++)
		{
			UART_transmit(buffer[i]);
		}
*/

		head++;										//incrementing head pointer

		UART_transmit('\r');
		UART_transmit('\n');

		if(head == (base_addr + 63))				//enables interrupt after every 64th sample
		{
			DMA_DCR0 |= DMA_DCR_EINT_MASK;
			__enable_irq();
		}

		// Prints DMA values stores in buffer
		sprintf(buffer, "%d", *head);
		for(int i=0; i<6; i++)
		{
			UART_transmit(buffer[i]);
		}
		UART_transmit('\t');

	}
    return 0;
}