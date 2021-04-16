/*
 * Interrupt.h
 *
 *  Created on: Apr 15, 2021
 *      Author: rache
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "msp.h"

void PORT6_IRQHandler(void);
void PORT3_IRQHandler(void);
void debounce(void);

volatile uint8_t alarm;
volatile uint8_t buttonflag;

#endif /* INTERRUPT_H_ */