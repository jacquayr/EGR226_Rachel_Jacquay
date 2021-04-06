/*
 * LCD.h
 *
 *  Created on: Mar 21, 2021
 *      Author: Rachel Jacquay
 */

#ifndef LCD_H_
#define LCD_H_

#include "msp.h"

void LCD_Init(void);
void Pin_Init(void);
void delay_micro(uint32_t microsecond);
void delay_milli(uint32_t millisecond);
void PulseEnablePin(void);
void pushNibble(uint8_t nibble);
void pushByte(uint8_t byte);
void commandWrite(uint8_t command);
void dataWrite(uint8_t data);

#endif /* LCD_H_ */
