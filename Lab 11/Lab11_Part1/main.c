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
        flag = 0;                   // set flag to 0
        SysTick_Delay(333);         // delay 333 ms

        if (flag == 0) {            // if flag is still 0, that means the interrupt was never called
            P1->OUT &= ~BIT0;       // keep LED off
            SysTick_Delay(1);       // delay 1 ms
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
    // use SMCLK, UPmode, clear TAR, /8

    TIMER_A0->CCR[0] = 37509;       // set period
    TIMER_A0->CCR[1] = 37509 / 2;   // set duty cycle

    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;     // set TIMERA0.1 to out mode 7
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

    TIMER_A2->CTL |= 0x0214;                        // SMCLK, UP mode, /8, clear TAR
    TIMER_A2->CCR[0] = 65535;                       // set period to 65535
    TIMER_A2->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;     // set TIMERA2.1 to out mode 7
    TIMER_A2->CCTL[1] = 0x4910;                     // capture rising edge, CCI2A, enable capture interrupt and mode
    // sync capture, enable interrupt in NVIC vector, enable global interrupt
}

/*--------------------------------------------------------------
 * Function:        LED_init
 *
 * Description:     This function initializes the red LED on P1.0.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void LED_init(void) {
    // red LED = P1.0
    P1->SEL0 &= ~BIT0;      // set up as simple I/O
    P1->SEL1 &= ~BIT0;
    P1->DIR |= BIT0;        // set to output
}

/*--------------------------------------------------------------
 * Function:        TA2_N_IRQHandler
 *
 * Description:     This function is the interrupt handler for
 *                  TIMERA2.1. It turns the
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void TA2_N_IRQHandler(void) {
    // P5.6 = TIMER 2.1
    TIMER_A2->CCTL[1] &= ~1;            // clear TA2.1 interrupt flag

    currentedge = TIMER_A2->CCR[1];     // set current edge equal to what was captured
    period = currentedge - lastedge;    // set period equal to current edge - last edge
    lastedge = currentedge;             // set last edge equal to current edge

    flag = 1;       // set flag to 1 to tell user that the handler was accessed

    if ((35635 < period) && (period < 39375)) {     // check if period is between 35635 & 39375
        P1->OUT |= BIT0;                            // turn red LED on
        SysTick_Delay(33);                          // delay 33 ms
    }

    TIMER_A0->CCTL[1] &= ~(TIMER_A_CCTLN_CCIFG);    // clear TA2.1 interrupt flag
}
