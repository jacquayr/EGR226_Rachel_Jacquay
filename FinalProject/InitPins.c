/*
 * InitPins.c
 *
 *  Created on: Apr 11, 2021
 *      Author: rache
 */

#include "InitPins.h"

/*--------------------------------------------------------------
 * Function:        LED_init
 *
 * Description:     This function initializes the red and green
 *                  LEDs for the door.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void LED_init(void) {
    // red LED = P1.0
    P1->SEL0 &= ~BIT0;
    P1->SEL1 &= ~BIT0;
    P1->DIR |= BIT0;
    P1->OUT |= BIT0;

    // green LED = P2.1
    P2->SEL0 &= ~BIT1;
    P2->SEL1 &= ~BIT1;
    P2->DIR |= BIT1;
    P2->OUT &= ~BIT1;
}

/*--------------------------------------------------------------
 * Function:        DC_init
 *
 * Description:     This function initializes the DC motor.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void DC_init(void) {
    P2->SEL0 |= BIT7;   // configure P2.7 for PWM
    P2->SEL1 &= ~BIT7;
    P2->DIR |= BIT7;

    // TimerA 0.4 for P2.7
    TIMER_A0->CCR[0] |= 30000;                      // set period amount
    TIMER_A0->CCTL[4] |= TIMER_A_CCTLN_OUTMOD_7;    // set to outmode 7
    TIMER_A0->CCR[4] = 0;                           // set duty cycle
    TIMER_A0->CTL |= 0x0214;                        // set to SMCLK, up mode, clear TAR to start

    P4->SEL0 &= ~0x7F;      // enable simple I/O
    P4->SEL1 &= ~0x7F;
    P4->DIR &= ~0x7F;       // set as input
    P4->REN |= 0x0F;        // enable resistor
    P4->OUT |= 0x0F;        // enable pull up resistor
}