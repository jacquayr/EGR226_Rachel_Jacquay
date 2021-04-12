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
}

/*--------------------------------------------------------------
 * Function:        button_init
 *
 * Description:     This function initializes the two buttons
 *                  with interrupts.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void Button_init(void) {
    // red = P6.4
    // blue = P6.5
    P6->SEL0 &= ~0x30;      // set as simple I/O
    P6->SEL1 &= ~0x30;
    P6->DIR &= ~0x30;       // set as input
    P6->REN |= 0x30;        // enable resistor
    P6->OUT |= 0x30;        // enable pull up
    P6->IE |= 0x30;         // set pin interrupt to trigger when it goes from high -> low
    P6->IES |= 0x30;        // enable interrupt for P6.1, P6.6, P6.7
    P6->IFG = 0;            // set flag to 0
}

/*--------------------------------------------------------------
 * Function:        Servo_init
 *
 * Description:     This function initializes the Servo motor.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void Servo_init(void) {
    P2->SEL0 |= BIT6;   // configure P2.6 for PWM
    P2->SEL1 &= ~BIT6;
    P2->DIR |= BIT6;

    // TimerA 0.3 for P2.6
    TIMER_A0->CCR[0] |= 20000;                      // set period amount
    TIMER_A0->CCTL[3] |= TIMER_A_CCTLN_OUTMOD_7;    // set to outmode 7
    TIMER_A0->CCR[3] = 1500;                           // set duty cycle
    TIMER_A0->CTL |= 0x0214;                        // set to SMCLK, up mode, clear TAR to start
}
