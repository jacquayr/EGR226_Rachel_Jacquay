/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            03/17/2021
 * Project:         Lab 8 Part 2
 * File:            main_part2.c
 * Description:     This program used Timer A, an Octocoupler, and a resistor to
 *                  create a pseudo PWM analog voltage for the DC motor. The variable
 *                  called 'change' was used to change the duty cycles of the PWM and alter
 *                  the speed of the DC motor while the code was in halt mode.
-----------------------------------------------------------------------------------------*/

// libraries
#include "msp.h"

void PWM_init(void);    // function prototype

volatile int change;    // variable to change

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	PWM_init();

	int DC = change;     // set DC equal to change variable

	while(1) {
	    TIMER_A0->CCR[1] = DC;      // set the duty cycle and send to CCR1 on Timer A
	}
}

/*--------------------------------------------------------------
 * Function:        PWM_init
 *
 * Description:     This function initializes the pins on the
 *                  MSP432 that are connected to the DC motor.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void PWM_init(void) {
    P2->SEL0 |= BIT4;       // configure P2.4 for PWM
    P2->SEL1 &= ~BIT4;
    P2->DIR |= BIT4;        // set as output
    P2->OUT &= ~BIT4;       // set as low

    TIMER_A0->CCR[0] |= 30000;                      // set period amount
    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_OUTMOD_7;    // set to mode 7
    TIMER_A0->CCR[1] |= 500;                        // set duty cycle amount
    TIMER_A0->CTL |= 0x0214;                        // set to SMCLK, up mode, clear TAR to start
}
