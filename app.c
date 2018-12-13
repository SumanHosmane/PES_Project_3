/*
 * app.c
 *
 *  Created on: Dec 12, 2018
 *      Author: Mohit
 */

#include "app.h"

void app()
{
	*head = fabs(*head);				//getting absolute value of adc_value
	if(*head > peak)						//checking for new peak
	{
		peak = *head;
		peak_flag = 1;
	}
}