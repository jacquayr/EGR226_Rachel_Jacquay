/*
 * SysTick.c
 *
 *  Created on: Mar 21, 2021
 *      Author: Rachel Jacquay
 */

#include "SysTick.h"

/*--------------------------------------------------------------
 * Function:        SysTick_Init
 * Description:     Initialize the SysTick delay component.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void SysTick_Init(void) {
    SysTick->CTRL = 0;              // disable SysTick during step
    SysTick->LOAD = 0x00FFFFFF;     // max reload value
    SysTick->VAL = 0;               // any write to current value clears it
    SysTick->CTRL = 0x00000005;     // enable SysTick (3 MHz) -> no interrupts
}


/*--------------------------------------------------------------
 * Function:        SysTick_Delay
 * Description:     Add a delay to make sure the button is
 *                  being pressed and not just bouncing.
 *
 * Inputs:          (unsigned 16-bit int) delay: integer to set the
 *                  length of the timer delay
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void SysTick_Delay(uint16_t delay) {
    SysTick->LOAD = ((delay * 3000) - 1);           // delay for 1 millisecond per delay value
    SysTick->VAL = 0;                               // any write to current value clears it
    while ((SysTick->CTRL & 0x00010000) == 0);      // wait for flag to be set
}
