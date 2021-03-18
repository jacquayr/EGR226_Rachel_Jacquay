/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            03/17/2021
 * Project:         Lab 8 Part 3
 * File:            main_part3.c
 * Description:     This program uses Timer A, an Octocoupler, and the keypad to create
 *                  a pseudo PWM analog voltage for the DC motor. The speed of the motor
 *                  was changed by the input of the keypad, which then changed the duty
 *                  cycle of Timer A, which then made the motor slow down or speed up.
-----------------------------------------------------------------------------------------*/

// libraries
#include "msp.h"
#include <stdio.h>

// function prototypes
void SysTick_Init(void);
void SysTick_Delay(uint8_t delay);
uint8_t Read_Keypad(void);
void Print_Keys(void);
void pin_init(void);

volatile int DC = 0;                // global volatile variables
volatile int period = 30000;

uint8_t num, pressed;       // global unsigned 8 bit variables

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    pin_init();                                     // initializations
    SysTick_Init();

    printf("Please enter a key: \n");       // tell user to press a key

    while(1) {
        pressed = Read_Keypad();            // call read keypad function and return 1 for pressed

        if (pressed) {                      // if a 1 is returned
            Print_Keys();                   // print the pressed key
            SysTick_Delay(1);               // delay for 1 ms
        }

        if (num <= 9) {                     // if number pressed is less than or equal to 9
            DC = period * (num * 0.1);      // set duty cycle to match the intensity of whichever button was pressed
        }

        else if (num == 10) {               // if number pressed is *
            DC = period * 0.99;             // set duty cycle to the max speed
        }

        else if (num == 11) {               // if number pressed is 0
            DC = 0;                         // set duty cycle to 0 (turn motor off)
        }

        TIMER_A0->CCR[1] = DC;              // send the duty cycle to Timer A's CCR1
    }
}

/*--------------------------------------------------------------
 * Function:        pin_init
 *
 * Description:     This function initializes the pins on the
 *                  MSP432 that are connected to the DC motor
 *                  and the keypad.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void pin_init(void) {
    P2->SEL0 |= BIT4;   // configure P2.4 for PWM
    P2->SEL1 &= ~BIT4;
    P2->DIR |= BIT4;

    TIMER_A0->CCR[0] |= period;                     // set period amount
    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_OUTMOD_7;    // set to outmode 7
    TIMER_A0->CCR[1] = DC;                          // set duty cycle
    TIMER_A0->CTL |= 0x0214;                        // set to SMCLK, up mode, clear TAR to start

    P4->SEL0 &= ~0x7F;      // enable simple I/O
    P4->SEL1 &= ~0x7F;
    P4->DIR &= ~0x7F;       // set as input
    P4->REN |= 0x0F;        // enable resistor
    P4->OUT |= 0x0F;        // enable pull up resistor
}

/*--------------------------------------------------------------
 * Function:        SysTick_Init
 *
 * Description:     This function initializes the SysTick
 *                  component of the code.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void SysTick_Init(void) {
    SysTick->CTRL = 0;              // disable during step
    SysTick->LOAD = 0x00FFFFFF;     // max reload value
    SysTick->VAL = 0;               // any write to current value clears it
    SysTick->CTRL = 0x00000005;     // enable SysTick (3 MHz) - no interrupts
}

/*--------------------------------------------------------------
 * Function:        SysTick_Delay
 *
 * Description:     This function is used in the code as a
 *                  delay in milliseconds to allow the MSP432
 *                  and LCD to connect correctly.
 *
 * Inputs:          (unsigned 8 bit integer) delay: this
 *                  variable takes in a value from the function
 *                  that calls the delay function and puts it
 *                  into the load register of SysTick to turn it
 *                  into a millisecond delay
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void SysTick_Delay(uint8_t delay) {
    SysTick->LOAD = ((3000 * delay) - 1);              // delay for 1 millisecond per delay value
    SysTick->VAL = 0;                               // any write to current value clears it
    while ((SysTick->CTRL & 0x00010000) == 0);      // wait for flag to be set
}

/*--------------------------------------------------------------
 * Function:        Read_Keypad
 *
 * Description:     This function sets the columns and rows both
 *                  to inputs, then makes one column at a time
 *                  an output and sets it to low. Then each of
 *                  the rows are read, one at a time, to determine
 *                  which connection (row & column) is low,
 *                  meaning that it is pressed.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
uint8_t Read_Keypad(void) {
    uint8_t col, row;

    for (col = 0; col < 3; col++) {         // check each column
        P4->DIR = 0x00;                     // set columns to inputs
        P4->DIR |= BIT(4 + col);            // set column 3 to output
        P4->OUT &= ~BIT(4 + col);           // set column 3 to low

        SysTick_Delay(10);                  // call systick delay
        row = P4->IN & 0x0F;                // read all rows

        while ( !(P4IN & BIT0) | !(P4IN & BIT1) |!(P4IN & BIT2) |!(P4IN & BIT3) );      // while the button is being held

        if (row != 0x0F)    // if something is pressed
            break;          // break out of for loop
    }

    P4->DIR = 0x00;         // set columns in inputs

    if (col == 3)           // nothing is pressed, return 0
        return 0;

    if (row == 0x0E)        // key in row 0
        num = col + 1;
    if (row == 0x0D)        // key in row 1
        num = 3 + col + 1;
    if (row == 0x0B)        // key in row 2
        num = 6 + col + 1;
    if (row == 0x07)        // key in row 3
        num = 9 + col + 1;

    return 1;               // return 1 for something is pressed
}

/*--------------------------------------------------------------
 * Function:        Print_Keys
 *
 * Description:     This function takes the value from 'num'
 *                  from the Read_Keypad function, and tells the
 *                  user which key they pressed.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void Print_Keys(void) {
    if (num >= 1 && num <= 9) {               // if num is 1-9
        printf("Key pressed: %d\n", num);     // print value of num
    }

    if (num == 10) {                          // if num is 10
        printf("Key pressed: *\n");           // print *
    }

    if (num == 11) {                          // if num is 11
        printf("Key pressed: 0\n");           // print 0
    }

    if (num == 12) {                          // if num is 12
        printf("Key pressed: #\n");           // print #
    }
}
