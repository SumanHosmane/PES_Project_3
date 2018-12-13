#ifndef HEADERS_DMA_H_
#define HEADERS_DMA_H_

#include <MKL25Z4.h>
#include <stdio.h>
#include <stdint.h>

#define ADC_reg_addr 0x4003B010

int16_t* base_addr;
int16_t* head;
char peak_buffer[6];

void DMA_config();
void DMA0_IRQHandler();

#endif /* HEADERS_DMA_H_ */
