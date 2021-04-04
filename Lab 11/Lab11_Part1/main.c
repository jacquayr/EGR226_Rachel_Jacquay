/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            04/04/2021
 * Project:         Lab 11 Parts 1 + 2
 * File:            main.c
 * Description:     This lab works with IR emitter and receiver to send out and read a
 *                  10Hz signal coming from the emitter. If the receiver gets the signal,
 *                  the red LED on P1.0 is turned on. If not, the red LED is off.
-----------------------------------------------------------------------------------------*/

// libraries
#include "msp.h"
#include "stdio.h"
#include "SysTick.h"

// globals
volatile uint16_t lastedge, currentedge, period, flag;

// function prototypes
void emitter_init(void);
void receiver_init(void);
void LED_init(void);
void TA2_N_IRQHandler(void);

void main(void) {
    __disable_irq();                                // disable irq
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    emitter_init();                                 // initializations
    receiver_init();
    LED_init();
    SysTick_Init();

    NVIC_EnableIRQ(TA2_N_IRQn);     // NVIC call
    __enable_irq();                 // enable irq

    while(1) {
        flag = 0;
        SysTick_Delay(333);

        if (flag == 0) {
            P1->OUT &= ~BIT0;
            flag = 1;
            SysTick_Delay(1);
        }
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

    TIMER_A0->CCR[0] = 37509;       // set period
    TIMER_A0->CCR[1] = 37509 / 2;   // set duty cycle

    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;
}

/*--------------------------------------------------------------
 * Function:        receiever_init
 *
 * Description:     This function initializes the receiver LED
 *                  so that it can receive the 10Hz blink.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void receiver_init(void) {
    // P5.6 = TIMER 2.1
    P5->SEL0 |= BIT6;       // setting up receiver for TIMERA capture mode
    P5->SEL1 &= ~BIT6;
    P5->DIR &= ~BIT6;

    TIMER_A2->CTL |= 0x0214;
    TIMER_A2->CCR[0] = 65535;
    TIMER_A2->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A2->CCTL[1] = 0x4910;
}

/*--------------------------------------------------------------
 * Function:        LED_init
 *
 * Description:
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
}

/*--------------------------------------------------------------
 * Function:        TA2_N_IRQHandler
 *
 * Description:
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void TA2_N_IRQHandler(void) {
    // P5.6 = TIMER 2.1
    TIMER_A2->CCTL[1] &= ~1;

    currentedge = TIMER_A2->CCR[1];
    period = currentedge - lastedge;
    lastedge = currentedge;

    flag = 1;

    if ((35635 < period) && (period < 39375)) {
        SysTick_Delay(1);
        if ((35635 < period) && (period < 39375)) {
            P1->OUT |= BIT0;
            SysTick_Delay(33);
        }
    }

    TIMER_A0->CCTL[1] &= ~(TIMER_A_CCTLN_CCIFG);
}
