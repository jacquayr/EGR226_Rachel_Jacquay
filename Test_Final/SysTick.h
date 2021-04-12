/*
 * SysTick.h
 *
 *  Created on: Mar 21, 2021
 *      Author: Rachel Jacquay
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "msp.h"

void SysTick_Init(void);
void SysTick_Delay(uint16_t delay);

#endif /* SYSTICK_H_ */
