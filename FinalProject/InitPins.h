/*
 * InitPins.h
 *
 *  Created on: Apr 11, 2021
 *      Author: rache
 */

#ifndef INITPINS_H_
#define INITPINS_H_

#include "msp.h"

void LED_init(void);
void DC_init(void);
void Button_init(void);
void Servo_init(void);
void RGB_init(void);
void ADCsetup(void);

#endif /* INITPINS_H_ */
