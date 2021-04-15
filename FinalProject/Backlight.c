/*
 * Backlight.c
 *
 *  Created on: Apr 15, 2021
 *      Author: rache
 */

#include "Backlight.h"

/*--------------------------------------------------------------
 * Function:        changeBL
 *
 * Description:     This function changes the backlight of the
 *                  LCD.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void changeBL(void) {
    float result = 0.0;

    ADC14->CTL0 |= 1;                           // start conversion
    while(!ADC14->IFGR0);                       // wait until conversion is complete
    result = ADC14->MEM[5];                     // immediately store value in variable
    TIMER_A1->CCR[1] = result * 0.0609;         // set duty cycle
}
