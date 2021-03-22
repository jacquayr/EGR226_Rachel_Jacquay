/*
 * Keypad.h
 *
 *  Created on: Mar 21, 2021
 *      Author: Rachel Jacquay
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "msp.h"

void Keypad_Init(void);
uint8_t Read_Keypad(void);
void Print_Keys(void);

uint8_t num, pressed;       // global variables

#endif /* KEYPAD_H_ */