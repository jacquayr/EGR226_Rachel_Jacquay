/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            03/17/2021
 * Project:         Lab 8 Part 2
 * File:            main_part2.c
 * Description:     This program
-----------------------------------------------------------------------------------------*/

#include "msp.h"
#include <stdio.h>

void PWM_init(void);

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	PWM_init();

	int DC = 5000;

	while(1) {
	    TIMER_A0->CCR[1] = DC;
	}
}

void PWM_init(void) {
    P2->SEL0 |= BIT4;
    P2->SEL1 &= ~BIT4;
    P2->DIR |= BIT4;

    TIMER_A0->CCR[0] |= 7500 - 1;
    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0->CCR[1] |= 500;
    TIMER_A0->CTL |= 0x0214;
}
