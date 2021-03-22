/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            03/24/2021
 * Project:         Lab 9 Part 1
 * File:            main_part1.c
 * Description:     This program used Timer A, an Optocoupler, a resistor, three push
 *                  buttons, SysTick, and wires to create a pseudo PWM analog voltage for
 *                  the DC motor. GPIO interrupts were used for the push buttons.
-----------------------------------------------------------------------------------------*/

// libraries
#include "msp.h"
#include <stdio.h>

// function prototypes
void pin_init(void);
void SysTick_Init(void);
void SysTick_Delay(uint16_t delay);
void debounce(void);
void PORT6_IRQHandler(void);

volatile uint16_t DC = 0;    // volatile variables
volatile uint8_t flag = 0;

void main(void)
{
    __disable_irq();                                // disable interrupts during initializations
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    pin_init();                                     // initializations
    SysTick_Init();

    NVIC_EnableIRQ(PORT6_IRQn);     // NVIC call
    __enable_irq();                 // enable interrupt request

    while (1) {     // always

    }
}

/*--------------------------------------------------------------
 * Function:        pin_init
 *
 * Description:     This function initializes the pins on the
 *                  MSP432 that are connected to the DC motor
 *                  and the ones used for PWM.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void pin_init(void) {
    P2->SEL0 |= BIT4;       // configure P2.4 for PWM
    P2->SEL1 &= ~BIT4;
    P2->DIR |= BIT4;        // set as output
    P2->OUT &= ~BIT4;       // set as low

    TIMER_A0->CCR[0] |= 30000;                      // set period amount
    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_OUTMOD_7;    // set to mode 7
    TIMER_A0->CCR[1] |= 0;                          // set duty cycle amount
    TIMER_A0->CTL |= 0x0214;                        // set to SMCLK, up mode, clear TAR to start

    // red = P6.6
    // white = P6.7
    // black = P6.1

    P6->SEL0 &= ~0xC2;      // set as simple I/O
    P6->SEL1 &= ~0xC2;
    P6->DIR &= ~0xC2;       // set as input
    P6->REN |= 0xC2;        // enable resistor
    P6->OUT |= 0xC2;        // enable pull up
    P6->IE |= 0xC2;         // set pin interrupt to trigger when it goes from high -> low
    P6->IES |= 0xC2;        // enable interrupt for P6.1, P6.6, P6.7
    P6->IFG = 0;            // set flag to 0
}

/*--------------------------------------------------------------
 * Function:        SysTick_Init
 *
 * Description:     Initialize the SysTick delay component.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void SysTick_Init(void) {
    SysTick->CTRL = 0;              // disable SysTick during step
    SysTick->LOAD = 0x00FFFFFF;     // max reload value
    SysTick->VAL = 0;               // any write to current value clears it
    SysTick->CTRL = 0x00000005;     // enable SysTick (3 MHz) -> no interrupts
}

/*--------------------------------------------------------------
 * Function:        SysTick_Delay
 *
 * Description:     This function takes in the variable 'delay'
 *                  from the function that called it, and turns
 *                  it into a millisecond delay.
 *
 * Inputs:          (unsigned 16-bit int) delay: integer to set the
 *                  length of the timer delay
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void SysTick_Delay(uint16_t delay) {
    SysTick->LOAD = ((delay * 3000) - 1);           // delay for 1 millisecond per delay value
    SysTick->VAL = 0;                               // any write to current value clears it
    while ((SysTick->CTRL & 0x00010000) == 0);      // wait for flag to be set
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
    if (((P6->IN & BIT6) != BIT6) || ((P6->IN & BIT7) != BIT7) || ((P6->IN & BIT1) != BIT1)) {          // if any switch is pressed
        SysTick_Delay(100);                                                                             // delay for 100 ms

        if (((P6->IN & BIT6) != BIT6) || ((P6->IN & BIT7) != BIT7) || ((P6->IN & BIT1) != BIT1)) {      // check switch again
            flag = 1;                                                                                   // set flag to 1
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
    // red = P6.6 --> stop
    // white = P6.7 --> inc 10%
    // black = P6.1 --> dec 10%

    if (P6->IFG & 0xC2) {       // if port 6 interrupts were changed and the flag was set
        debounce();             // call debounce function

        while (flag == 1) {                         // do while flag is set from debounce function
            if ((P6->IN & BIT6) != BIT6) {          // if red button is pressed
                DC = 0;                             // set duty cycle to 0
                TIMER_A0->CCR[1] = DC;              // set the duty cycle and send to CCR1 on Timer A
                SysTick_Delay(1);                   // call Systick delay function
            }

            else if ((P6->IN & BIT7) != BIT7) {     // if white button is pressed
                if (DC >= 30000) {                  // if duty cycle is at or above 30000
                    DC = 30000;                     // keep duty cycle at 30000
                }

                else {                              // if duty cycle is not at or above 30000
                    DC += 3000;                     // increase by 10% of period (3000)
                }

                TIMER_A0->CCR[1] = DC;              // set the duty cycle and send to CCR1 on Timer A
                SysTick_Delay(1);                   // call Systick delay function
            }

            else if ((P6->IN & BIT1) != BIT1) {     // if black button is pressed
                if (DC <= 0) {                      // if duty cycle is at or below 0
                    DC = 0;                         // keep duty cycle at 0
                }

                else {                              // if duty cycle is not at or below 0
                    DC -= 3000;                     // decrease by 10% of period (3000)
                }

                TIMER_A0->CCR[1] = DC;              // set the duty cycle and send to CCR1 on Timer A
                SysTick_Delay(1);                   // call Systick delay function
            }
            flag = 0;                               // reset flag to 0
        }
    }

    P6->IFG &= ~0xC2;       // reset interrupt flag
}