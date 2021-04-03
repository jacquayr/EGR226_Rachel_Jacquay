/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            04/04/2021
 * Project:         Lab 11 Parts 1 + 2
 * File:            main.c
 * Description:
-----------------------------------------------------------------------------------------*/

// libraries
#include "msp.h"
#include <stdio.h>

// function prototypes
void emitter_init(void);
void receive_init(void);
void LED_init(void);
void T32_INT2_IRQHandler(void);

// globals
uint8_t detect10Hz;
volatile uint16_t lastedge, currentedge, period;

void main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    emitter_init();                                 // initializations
    receive_init();
    LED_init();

    NVIC_EnableIRQ(T32_INT2_IRQn);     // NVIC call
    __enable_irq();                    // enable irq

    while(1) {

    }
}

/*--------------------------------------------------------------
 * Function:        emitter_init
 *
 * Description:     This function initializes the emitter LED
 *                  so that it blinks at 10Hz.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void emitter_init(void) {
    P2->SEL0 |= BIT4;   // P2.4 set TA0.1
    P2->SEL1 &= ~BIT4;
    P2->DIR |= BIT4;

    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR | TIMER_A_CTL_ID__8;

    TIMER_A0->CCR[0] = 37509;
    TIMER_A0->CCR[1] = 37509 / 2;

    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;
}

/*--------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void receive_init(void) {
    P6->SEL0 |= BIT6;
    P6->SEL1 &= ~BIT6;
    P6->DIR &= ~BIT6;

    TIMER_A0->CTL |= 0x0214;
    TIMER_A0->CCTL[2] = 0x4910;
}

/*--------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void LED_init(void) {
    P2->SEL0 &= ~BIT0;
    P2->SEL1 &= ~BIT0;
    P2->DIR |= BIT0;
    P2->SEL0 |= BIT0;

    TIMER32_2->CONTROL = 0b11100011;
}

/*--------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void TA0_N_IRQHandler(void) {
    TIMERA0->CCTL[2] &= ~1;
    currentedge = TIMERA0_->CCR[2];
    period = currentedge - lastedge;
    lastedge = currentedge;

    if ((39635 < period) && (period < 39375)) {
        detect10Hz = 1;
    }

    if (detect10Hz == 1) {
        TIMER32_2->INTCLR = 1;
        P2->OUT ^= BIT0;
        TIMER32_2->LOAD = 9000000;
    }
}
