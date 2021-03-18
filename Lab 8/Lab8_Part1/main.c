/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            03/17/2021
 * Project:         Lab 8 Part 1
 * File:            main_part1.c
 * Description:     This program
-----------------------------------------------------------------------------------------*/

#include "msp.h"
#include <stdio.h>

void pin_init(void);
void SysTick_Init(void);
void SysTick_Delay(uint8_t delay);

volatile float DC;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    SysTick_Init();
    pin_init();

    float Ton, Toff, period;

    period = 100;

    while (1) {
        Ton = (DC / 100.0) * period;
        Toff = period - Ton;

        P3->OUT |= BIT0;
        SysTick_Delay(Ton);

        P3->OUT &= ~BIT0;
        SysTick_Delay(Toff);
    }
}

/*--------------------------------------------------------------
 * Function:        pin_init
 * Description:     This function initializes the pins on the
 *                  MSP432 that are connected to the LCD.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void pin_init(void) {
    P3->SEL0 &= ~BIT0;
    P3->SEL1 &= ~BIT0;
    P3->DIR |= BIT0;
    P3->OUT &= ~BIT0;
}

/*--------------------------------------------------------------
 * Function:        SysTick_Init
 * Description:     This function initializes the SysTick
 *                  component of the code.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void SysTick_Init(void) {
    SysTick->CTRL = 0;              // disable during step
    SysTick->LOAD = 0x00FFFFFF;     // max reload value
    SysTick->VAL = 0;               // any write to current value clears it
    SysTick->CTRL = 0x00000005;     // enable SysTick (3 MHz) - no interrupts
}

/*--------------------------------------------------------------
 * Function:        SysTick_Delay
 *
 * Description:     This function is used in the code as a
 *                  delay in milliseconds to allow the MSP432
 *                  and LCD to connect correctly.
 *
 * Inputs:
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void SysTick_Delay(uint8_t delay) {
    SysTick->LOAD = ((3000 * delay) - 1);              // delay for 1 millisecond per delay value
    SysTick->VAL = 0;                               // any write to current value clears it
    while ((SysTick->CTRL & 0x00010000) == 0);      // wait for flag to be set
}
