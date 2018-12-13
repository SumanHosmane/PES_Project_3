/*
 * app.h
 *
 *  Created on: Dec 12, 2018
 *      Author: Mohit
 */

#ifndef HEADERS_APP_H_
#define HEADERS_APP_H_

#include <MKL25Z4.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

float alpha = 0.8;
int peak = 0;
int16_t* head;
uint8_t peak_flag;
int16_t adc_value;

void app(void);
#endif /* HEADERS_APP_H_ */
