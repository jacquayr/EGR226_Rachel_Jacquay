/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            03/17/2021
 * Project:         Lab 8 Part 3
 * File:            main_part3.c
 * Description:     This program
-----------------------------------------------------------------------------------------*/

#include "msp.h"
#include <stdio.h>

void SysTick_Init(void);
void SysTick_Delay(uint8_t delay);

void keypad_init(void);
uint8_t Read_Keypad(void);
void Print_Keys(void);
void pin_init(void);

volatile int DC = 0;
volatile int period = 30000;

uint8_t num, pressed;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    pin_init();
    SysTick_Init();
    keypad_init();

    printf("Please enter a key: \n");

    while(1) {
        pressed = Read_Keypad();

        if (pressed) {
            Print_Keys();
            SysTick_Delay(1);

            if (num == 11) {
                DC = 0;
            }
        }

        else if (num <= 9) {
            DC = period * (num * 0.1);
        }

        TIMER_A0->CCR[1] = DC;
    }
}

void pin_init(void) {
    P2->SEL0 |= BIT4;
    P2->SEL1 &= ~BIT4;
    P2->DIR |= BIT4;

    TIMER_A0->CCR[0] |= period;
    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0->CCR[1] = DC;
    TIMER_A0->CTL |= 0x0214;
}

void SysTick_Init(void) {
    SysTick->CTRL = 0;              // disable during step
    SysTick->LOAD = 0x00FFFFFF;     // max reload value
    SysTick->VAL = 0;               // any write to current value clears it
    SysTick->CTRL = 0x00000005;     // enable SysTick (3 MHz) - no interrupts
}

void SysTick_Delay(uint8_t delay) {
    SysTick->LOAD = ((3000 * delay) - 1);              // delay for 1 millisecond per delay value
    SysTick->VAL = 0;                               // any write to current value clears it
    while ((SysTick->CTRL & 0x00010000) == 0);      // wait for flag to be set
}

void keypad_init(void) {
    P4->SEL0 &= ~0x7F;
    P4->SEL1 &= ~0x7F;
    P4->DIR &= ~0x7F;
    P4->REN |= 0x0F;
    P4->OUT |= 0x0F;
}

uint8_t Read_Keypad(void) {
    uint8_t col, row;

    for (col = 0; col < 3; col++) {         // check each column
        P4->DIR = 0x00;                     // set columns to inputs
        P4->DIR |= BIT(4 + col);            // set column 3 to output
        P4->OUT &= ~BIT(4 + col);           // set column 3 to low

        SysTick_Delay(10);                  // call systick delay
        row = P4->IN & 0x0F;                // read all rows

        while ( !(P4IN & BIT0) | !(P4IN & BIT1) |!(P4IN & BIT2) |!(P4IN & BIT3) );      // while nothing is pressed

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

    return 1;       // return 1 for something is pressed
}

void Print_Keys(void) {
    if (num >= 1 && num <= 9) {               // if the num value is 1-9
        printf("Key pressed: %d\n", num);     // print value of num
    }

    if (num == 10) {                          // if num val is 10
        printf("Key pressed: *\n");           // print *
    }

    if (num == 11) {                          // if num val is 11
        printf("Key pressed: 0\n");           // print 0
    }

    if (num == 12) {                          // if num val is 12
        printf("Key pressed: #\n");           // print #
    }
}
