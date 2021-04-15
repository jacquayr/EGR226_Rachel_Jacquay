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
    P6->IES |= 0x30;        // enable interrupt for P6.4, P6.5
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
    P2->SEL0 |= BIT5;   // configure P2.5 for PWM
    P2->SEL1 &= ~BIT5;
    P2->DIR |= BIT5;

    // TimerA 0.2 for P2.5
    TIMER_A0->CCTL[2] |= TIMER_A_CCTLN_OUTMOD_7;    // set to outmode 7
    TIMER_A0->CCR[2] = 9000;                        // set duty cycle
}

/*--------------------------------------------------------------
 * Function:        RGB_init
 *
 * Description:     This function initializes the RGB LED.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void RGB_init(void) {
    // green LED = P2.6 --> TIMER A 0.3
    // red LED = P5.6 --> TIMER A 2.1
    // blue LED = P6.6 --> TIMER A 2.3

    P2->SEL0 |= BIT6;
    P2->SEL1 &= ~BIT6;
    P2->DIR |= BIT6;

    TIMER_A0->CCTL[3] |= TIMER_A_CCTLN_OUTMOD_7;    // set to outmode 7
    TIMER_A0->CCR[3] = 0;                           // set duty cycle

    P5->SEL0 |= BIT6;
    P5->SEL1 &= ~BIT6;
    P5->DIR |= BIT6;

    TIMER_A2->CCR[0] = 30000;                           // set duty cycle
    TIMER_A2->CCTL[1] |= TIMER_A_CCTLN_OUTMOD_7;    // set to outmode 7
    TIMER_A2->CCR[1] = 0;                           // set duty cycle
    TIMER_A2->CTL |= 0x0214;                        // set to SMCLK, up mode, clear TAR to start

    P6->SEL0 |= BIT6;
    P6->SEL1 &= ~BIT6;
    P6->DIR |= BIT6;

    TIMER_A2->CCTL[3] |= TIMER_A_CCTLN_OUTMOD_7;    // set to outmode 7
    TIMER_A2->CCR[3] = 0;                           // set duty cycle
}

/*--------------------------------------------------------------
 * Function:        Back_init()
 *
 * Description:     This function initializes P7.7 with GPIO
 *                  interrupts.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void Back_init(void) {
    // backlight = P7.7
    P7->SEL0 |= BIT7;      // set as simple I/O
    P7->SEL1 &= ~BIT7;
    P7->DIR |= BIT7;       // set as input

    TIMER_A1->CCR[0] |= 1000;                          // set period amount
    TIMER_A1->CCTL[1] |= TIMER_A_CCTLN_OUTMOD_7;        // set to outmode 7
    TIMER_A1->CCR[1] = 500;                           // set duty cycle
    TIMER_A1->CTL |= 0x0214;                            // set to SMCLK, up mode, clear TAR to start
}