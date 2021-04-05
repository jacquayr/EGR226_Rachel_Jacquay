/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            04/04/2021
 * Project:         Lab 11 Parts 1 + 2
 * File:            main.c
 * Description:     This lab works with IR emitter and receiver to send out and read a
 *                  10Hz or 14Hz signal coming from the emitter. The red LED turns on for
 *                  10Hz, and the blue LED turns on for 14Hz.
-----------------------------------------------------------------------------------------*/

// libraries
#include "msp.h"
#include "stdio.h"
#include "SysTick.h"

// globals
volatile uint16_t lastedge, currentedge, period, flag, Bflag, red, blue;

// function prototypes
void emitter_init(void);
void receiver_init(void);
void LED_init(void);
void button_init(void);
void TA2_N_IRQHandler(void);
void PORT6_IRQHandler(void);

void main(void) {
    __disable_irq();                                // disable irq
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    emitter_init();                                 // initializations
    receiver_init();
    LED_init();
    SysTick_Init();
    button_init();

    NVIC_EnableIRQ(TA2_N_IRQn);     // NVIC call
    NVIC_EnableIRQ(PORT6_IRQn);     // NVIC call
    __enable_irq();                 // enable irq

    red = 0;
    blue = 0;

    while(1) {
        flag = 0;                   // set flag to 0
        red = 0;
        blue = 0;
        SysTick_Delay(333);         // delay 333 ms

        if (flag == 0) {            // if flag is still 0, that means the interrupt was never called
            P1->OUT &= ~BIT0;       // keep LEDs off
            P2->OUT &= ~BIT2;
            SysTick_Delay(1);       // delay 1 ms
        }

        if (red == 1) {
            P1->OUT |= BIT0;
            P2->OUT &= ~BIT2;
        }

        else if (blue == 1) {
            P2->OUT |= BIT2;
            P1->OUT &= ~BIT0;
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
 * Description:     This function initializes the LEDs on P1.0
 *                  and P2.2.
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

    // blue LED = P2.2
    P2->SEL0 &= ~BIT2;      // set up as simple I/O
    P2->SEL1 &= ~BIT2;
    P2->DIR |= BIT2;        // set to output
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
        P2->OUT &= ~BIT2;                           // turn blue LED off
        SysTick_Delay(33);                          // delay 33 ms
    }

    if ((15446 < period) && (period < 25055)) {     // check if period is between 15446 & 25055
        P2->OUT |= BIT2;                                 // turn blue LED on
        P1->OUT &= ~BIT0;                                // turn red LED off
        SysTick_Delay(33);                               // delay 33 ms
    }

    TIMER_A0->CCTL[1] &= ~(TIMER_A_CCTLN_CCIFG);    // clear TA2.1 interrupt flag
}

/*--------------------------------------------------------------
 * Function:        button_init
 *
 * Description:     This function initalizes the two buttons.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void button_init(void) {
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
 * Function:        debounce
 *
 * Description:     This function deals with the switch debounce
 *                  and determines if button is actually pressed.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void debounce(void) {
    if (((P6->IN & BIT4) != BIT4) || ((P6->IN & BIT5) != BIT5)) {          // if any switch is pressed
        SysTick_Delay(10);                                                 // delay for 10 ms

        if (((P6->IN & BIT4) != BIT4) || ((P6->IN & BIT5) != BIT5)) {      // check switch again
            Bflag = 1;                                                     // set flag to 1
        }
    }
}

/*--------------------------------------------------------------
 * Function:        PORT6_IRQHandler
 *
 * Description:     This function deals with the GPIO interrupt
 *                  and calls the debounce function, then checks
 *                  if any of buttons are pressed, and acts
 *                  accordingly.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void PORT6_IRQHandler(void) {
    // red = P6.4
    // blue = P6.5

    if (P6->IFG & 0x30) {       // if port 6 interrupts were changed and the flag was set
        debounce();             // call debounce function

        while (Bflag == 1) {                        // do while flag is set from debounce function
            if ((P6->IN & BIT4) != BIT4) {          // if red button is pressed
                TIMER_A0->CCR[0] = 37509;           // set period
                TIMER_A0->CCR[1] = 37509 / 2;       // set duty cycle
                red = 1;
                blue = 0;
            }

            else if ((P6->IN & BIT5) != BIT5) {     // if blue button is pressed
                TIMER_A0->CCR[0] = 26785;           // set period
                TIMER_A0->CCR[1] = 26785 / 2;       // set duty cycle
                blue = 1;
                red = 0;
            }

            Bflag = 0;                               // reset flag to 0
        }
    }

    P6->IFG &= ~0x30;       // reset interrupt flag
}