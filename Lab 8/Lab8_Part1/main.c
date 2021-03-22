/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            03/17/2021
 * Project:         Lab 8 Part 1
 * File:            main_part1.c
 * Description:     This program uses SysTick and the DC motor, as well as a MOSFET, to
 *                  create a pseudo PWM analog voltage for the DC motor. The variable
 *                  called 'DC' was used to change the duty cycles of the PWM and alter
 *                  the speed of the DC motor while the code was being run.
-----------------------------------------------------------------------------------------*/

// libraries
#include "msp.h"
#include <stdio.h>

// function prototypes
void pin_init(void);
void SysTick_Init(void);
void SysTick_Delay(uint8_t delay);

// global variables
volatile float DC;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    SysTick_Init();                                 // initialization function calls
    pin_init();

    float Ton, Toff, period;                        // declare variables

    period = 100;

    while (1) {
        Ton = (DC / 100.0) * period;                // on period is duty cycle divided by 100 times period
        Toff = period - Ton;                        // off period is period minus on period

        P3->OUT |= BIT0;                            // turn motor on
        SysTick_Delay(Ton);                         // delay for on period time

        P3->OUT &= ~BIT0;                           // turn motor off
        SysTick_Delay(Toff);                        // delay for off period time
    }
}

/*--------------------------------------------------------------
 * Function:        pin_init
 *
 * Description:     This function initializes the pins on the
 *                  MSP432 that are connected to the LCD.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void pin_init(void) {
    P3->SEL0 &= ~BIT0;      // enable simple I/O
    P3->SEL1 &= ~BIT0;
    P3->DIR |= BIT0;        // set as output
    P3->OUT &= ~BIT0;       // set to low
}

/*--------------------------------------------------------------
 * Function:        SysTick_Init
 *
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
 * Inputs:          (unsigned 8 bit integer) delay: this
 *                  variable takes in a value from the function
 *                  that calls the delay function and puts it
 *                  into the load register of SysTick to turn it
 *                  into a millisecond delay
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void SysTick_Delay(uint8_t delay) {
    SysTick->LOAD = ((3000 * delay) - 1);           // delay for 1 millisecond per delay value
    SysTick->VAL = 0;                               // any write to current value clears it
    while ((SysTick->CTRL & 0x00010000) == 0);      // wait for flag to be set
}