#include "uart.h"

void UART_config()
{
	//Clock settings
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM_SOPT2 |= 0x4000000u;								//clock source selection

	//Disable Tx and Rx before setting baud rates
	UART0_C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK);

	//baud rate settings - 57600
	UART0_BDH &= 0x00;
	UART0_BDL = 0x1A;
	UART0_C4 |= 0x0F;
	UART0_C1 &= 0x00;

	//UART0 RX/TX is connected to pins 27/28, PTA1/PTA2 (ALT2)
	PORTA_PCR1 = PORT_PCR_MUX(2);
	PORTA_PCR2 = PORT_PCR_MUX(2);

	//Enabling Tx and Rx for polling
	UART0_C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK);
}

void UART_transmit(uint8_t data)							//function to print on UART terminal
{
	while(!(UART0_S1 & UART_S1_TDRE_MASK));
	UART0_D = data;
}
