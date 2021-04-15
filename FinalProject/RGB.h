/*
 * RGB.h
 *
 *  Created on: Apr 12, 2021
 *      Author: rache
 */

#ifndef RGB_H_
#define RGB_H_

#include "msp.h"

void red(void);
void blue(void);
void green(void);
void updateBL(void);

volatile uint8_t redSum;
volatile uint8_t greenSum;
volatile uint8_t blueSum;
volatile uint8_t redOn;
volatile uint8_t greenOn;
volatile uint8_t blueOn;

#endif /* RGB_H_ */
