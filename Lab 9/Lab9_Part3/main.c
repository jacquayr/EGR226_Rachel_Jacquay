/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            03/24/2021
 * Project:         Lab 9 Part 3
 * File:            main_part3.c
 * Description:     This program uses two push buttons, the 7-segment delay, two resistors,
 *                  SysTick, and a bunch of wires to allow the user to press a push button
 *                  and change the number shown on the 7-second display. GPIO interrupts
 *                  are used in this code.
-----------------------------------------------------------------------------------------*/

// libraries
#include "msp.h"
#include <stdio.h>

// function prototypes
void SysTick_Init(void);
void SysTick_Delay(uint32_t delay);
void pin_init(void);
void debounce(void);
void PORT6_IRQHandler(void);
void changeCase(void);

volatile int32_t count = 0;    // volatile global variables
volatile uint8_t flag = 0;

void main(void)
{
    __disable_irq();                                // disable interrupts during initializations
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    pin_init();                                     // initializations
    SysTick_Init();

    NVIC_EnableIRQ(PORT6_IRQn);     // NVIC call
    __enable_irq();                 // enable interrupt request

    P4->OUT &= ~0x7F;   // turn all off
    P4->OUT |= 0x3F;    // turn on certain bits

    while (1) {

    }
}

/*--------------------------------------------------------------
 * Function:        SysTick_Init
 *
 * Description:     This function initializes the SysTick delay
 *                  component.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void SysTick_Init(void) {
    SysTick->CTRL = 0;              // disable SysTick during step
    SysTick->LOAD = 0x00FFFFFF;     // max reload value
    SysTick->VAL = 0;               // any write to current value clears it
    SysTick->CTRL = 0x00000005;     // enable SysTick (3 MHz) -> NO INTERRUPTS
}


/*--------------------------------------------------------------
 * Function:        SysTick_Delay
 *
 * Description:     This function takes in the variable 'delay'
 *                  from the function that called it, and turns
 *                  it into a millisecond delay.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void SysTick_Delay(uint32_t delay) {
    SysTick->LOAD = (delay * 3000)- 1;              // delay for 1 millisecond per delay value
    SysTick->VAL = 0;                               // any write to current value clears it
    while ((SysTick->CTRL & 0x00010000) == 0);      // wait for flag to be set
}

/*--------------------------------------------------------------
 * Function:        pin_init
 *
 * Description:     This function initializes the pins for the
 *                  7-segment display and push buttons.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void pin_init(void) {
    // DISPLAY
    P4->SEL0 &= ~0x7F;      // set as simple I/O
    P4->SEL1 &= ~0x7F;
    P4->DIR |= 0x7F;        // set to output
    P4->OUT &= ~0x7F;       // set to low

    // red = P6.6
    // white = P6.7
    // black = P6.1

    // BUTTONS
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
 * Function:        changeCase
 *
 * Description:     This function changes the case based on the
 *                  count variable value and turns certain bits
 *                  on so that the number display in order.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void changeCase(void) {
    switch (count) {        // switch statement

    case 0 :                // case for 0
        P4->OUT &= ~0x7F;   // turn all off
        P4->OUT |= 0x3F;    // turn on certain bits
        break;              // break

    case 1 :                // case for 1
        P4->OUT &= ~0x7F;   // turn all off
        P4->OUT |= 0x06;    // turn on certain bits
        break;              // break

    case 2 :                // case for 2
        P4->OUT &= ~0x7F;   // turn all off
        P4->OUT |= 0x5B;    // turn on certain bits
        break;              // break

    case 3 :                // case for 3
        P4->OUT &= ~0x7F;   // turn all off
        P4->OUT |= 0x4F;    // turn on certain bits
        break;              // break

    case 4 :                // case for 4
        P4->OUT &= ~0x7F;   // turn all off
        P4->OUT |= 0x66;    // turn on certain bits
        break;              // break

    case 5 :                // case for 5
        P4->OUT &= ~0x7F;   // turn all off
        P4->OUT |= 0x6D;    // turn on certain bits
        break;              // break

    case 6 :                // case for 6
        P4->OUT &= ~0x7F;   // turn all off
        P4->OUT |= 0x7D;    // turn on certain bits
        break;              // break

    case 7 :                // case for 7
        P4->OUT &= ~0x7F;   // turn all off
        P4->OUT |= 0x07;    // turn on certain bits
        break;              // break

    case 8 :                // case for 8
        P4->OUT &= ~0x7F;   // turn all off
        P4->OUT |= 0x7F;    // turn on certain bits
        break;              // break

    case 9 :                // case for 9
        P4->OUT &= ~0x7F;   // turn all off
        P4->OUT |= 0x6F;    // turn on certain bits
        break;              // break

    default :               // default case
        P4->OUT &= ~0x7F;   // turn all off
        P4->OUT |= 0x3F;    // turn on certain bits
        count = 0;          // reset count to 0
    }
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
        SysTick_Delay(10);                                                                              // delay for 10 ms

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
                count = 0;                          // set case to 0
                changeCase();                       // call change case function
            }

            else if ((P6->IN & BIT7) != BIT7) {     // if white button is pressed
                count++;                            // increment count first

                if (count > 9) {                    // if count is above 9
                    count = 0;                      // set count to 0
                }

                changeCase();                       // call change case function
            }

            else if ((P6->IN & BIT1) != BIT1) {     // if black button is pressed
                count--;                            // decrement count

                if (count < 0) {                    // if count is below 0
                    count = 9;                      // set count to 0
                }

                changeCase();                       // call change case function
            }
            flag = 0;                               // reset flag to 0
        }
    }

    P6->IFG &= ~0xC2;       // reset interrupt flag
}
