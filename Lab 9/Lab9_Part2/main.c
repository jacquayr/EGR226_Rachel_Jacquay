/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            03/24/2021
 * Project:         Lab 9 Part 2
 * File:            main_part2.c
 * Description:     This program used the 7-segment display, two resistors, a bunch of
 *                  wires, and SysTick, to interface the numbers 0 through 9 on the display.
 *                  SysTick interrupts were used.
-----------------------------------------------------------------------------------------*/

// libraries
#include "msp.h"
#include <stdio.h>

// function prototypes
void pin_init(void);
void SysTick_Init(void);
void SysTick_Delay(void);
void changeCase(void);

volatile uint32_t count = 0;    // volatile global variable

void main(void)
{
    __disable_irq();                                // disable interrupts
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    SysTick_Init();                                 // initializations
    pin_init();
    __enable_irq();                                 // enable interrupts

    while (1) {
        changeCase();           // call change case function

        if (count > 9) {        // if count is larger than 9
            count = 0;          // reset to 0 to restart
        }
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
    SysTick->CTRL = 0x00000007;     // enable SysTick (3 MHz) -> YES INTERRUPTS
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
void SysTick_Delay(void) {
    SysTick->LOAD = 3000000 - 1;                    // delay for 1 SECOND
    SysTick->VAL = 0;                               // any write to current value clears it
    while ((SysTick->CTRL & 0x00010000) == 0);      // wait for flag to be set
}

/*--------------------------------------------------------------
 * Function:        pin_init
 *
 * Description:     This function initializes the pins for the
 *                  7-segment display.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void pin_init(void) {
    P4->SEL0 &= ~0x7F;      // set as simple I/O
    P4->SEL1 &= ~0x7F;
    P4->DIR |= 0x7F;        // set to output
    P4->OUT &= ~0x7F;       // set to low
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
        P4->OUT |= 0x3F;    // turn on certain bits
        SysTick_Delay();    // delay 1 sec
        P4->OUT &= ~0x7F;   // turn all off
        break;              // break

    case 1 :                // case for 1
        P4->OUT |= 0x06;    // turn on certain bits
        SysTick_Delay();    // delay 1 sec
        P4->OUT &= ~0x7F;   // turn all off
        break;              // break

    case 2 :                // case for 2
        P4->OUT |= 0x5B;    // turn on certain bits
        SysTick_Delay();    // delay 1 sec
        P4->OUT &= ~0x7F;   // turn all off
        break;              // break

    case 3 :                // case for 3
        P4->OUT |= 0x4F;    // turn on certain bits
        SysTick_Delay();    // delay 1 sec
        P4->OUT &= ~0x7F;   // turn all off
        break;              // break

    case 4 :                // case for 4
        P4->OUT |= 0x66;    // turn on certain bits
        SysTick_Delay();    // delay 1 sec
        P4->OUT &= ~0x7F;   // turn all off
        break;              // break

    case 5 :                // case for 5
        P4->OUT |= 0x6D;    // turn on certain bits
        SysTick_Delay();    // delay 1 sec
        P4->OUT &= ~0x7F;   // turn all off
        break;              // break

    case 6 :                // case for 6
        P4->OUT |= 0x7D;    // turn on certain bits
        SysTick_Delay();    // delay 1 sec
        P4->OUT &= ~0x7F;   // turn all off
        break;              // break

    case 7 :                // case for 7
        P4->OUT |= 0x07;    // turn on certain bits
        SysTick_Delay();    // delay 1 sec
        P4->OUT &= ~0x7F;   // turn all off
        break;              // break

    case 8 :                // case for 8
        P4->OUT |= 0x7F;    // turn on certain bits
        SysTick_Delay();    // delay 1 sec
        P4->OUT &= ~0x7F;   // turn all off
        break;              // break

    case 9 :                // case for 9
        P4->OUT |= 0x6F;    // turn on certain bits
        SysTick_Delay();    // delay 1 sec
        P4->OUT &= ~0x7F;   // turn all off
        break;              // break

    default :               // default case
        P4->OUT &= ~0x7F;   // turn all bits off
        SysTick_Delay();    // delay 1 sec
        count = 0;          // reset count to 0
    }
}

/*--------------------------------------------------------------
 * Function:        SysTick_Handler
 *
 * Description:     This function deals with the SysTick
 *                  interrupt and increases the overall count.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void SysTick_Handler(void) {
    count++;    // increase count by 1
}
