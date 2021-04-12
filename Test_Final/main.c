/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            03/17/2021
 * Project:         Lab 8 Part 3
 * File:            main_part3.c
 * Description:     This program uses Timer A, an Octocoupler, and the keypad to create
 *                  a pseudo PWM analog voltage for the DC motor. The speed of the motor
 *                  was changed by the input of the keypad, which then changed the duty
 *                  cycle of Timer A, which then made the motor slow down or speed up.
-----------------------------------------------------------------------------------------*/

// libraries
#include "msp.h"
#include "SysTick.h"
#include "Keypad.h"
#include <stdio.h>

// function prototypes
void pin_init(void);

volatile int DC = 0;                // global volatile variables
volatile int period = 30000;

uint8_t num, pressed;       // global unsigned 8 bit variables

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    pin_init();                                     // initializations
    SysTick_Init();

    printf("Please enter a key: \n");       // tell user to press a key

    while(1) {
        pressed = Read_Keypad();            // call read keypad function and return 1 for pressed

        if (pressed) {                      // if a 1 is returned
            Print_Keys();                   // print the pressed key
            SysTick_Delay(1);               // delay for 1 ms
        }

        if (num <= 9) {                     // if number pressed is less than or equal to 9
            DC = period * (num * 0.1);      // set duty cycle to match the intensity of whichever button was pressed
        }

        else if (num == 10) {               // if number pressed is *
            DC = period * 0.99;             // set duty cycle to the max speed
        }

        else if (num == 11) {               // if number pressed is 0
            DC = 0;                         // set duty cycle to 0 (turn motor off)
        }

        TIMER_A0->CCR[4] = DC;              // send the duty cycle to Timer A's CCR1
    }
}

/*--------------------------------------------------------------
 * Function:        pin_init
 *
 * Description:     This function initializes the pins on the
 *                  MSP432 that are connected to the DC motor
 *                  and the keypad.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void pin_init(void) {
    P2->SEL0 |= BIT7;   // configure P2.4 for PWM
    P2->SEL1 &= ~BIT7;
    P2->DIR |= BIT7;

    TIMER_A0->CCR[0] |= period;                     // set period amount
    TIMER_A0->CCTL[4] |= TIMER_A_CCTLN_OUTMOD_7;    // set to outmode 7
    TIMER_A0->CCR[4] = DC;                          // set duty cycle
    TIMER_A0->CTL |= 0x0214;                        // set to SMCLK, up mode, clear TAR to start

    P4->SEL0 &= ~0x7F;      // enable simple I/O
    P4->SEL1 &= ~0x7F;
    P4->DIR &= ~0x7F;       // set as input
    P4->REN |= 0x0F;        // enable resistor
    P4->OUT |= 0x0F;        // enable pull up resistor
}
