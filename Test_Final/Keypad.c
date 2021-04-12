/*
 * Keypad.c
 *
 *  Created on: Mar 21, 2021
 *      Author: Rachel Jacquay
 */

#include "Keypad.h"
#include "SysTick.h"
#include <stdlib.h>
#include <stdio.h>

/*--------------------------------------------------------------
 * Function:        Keypad_Init
 * Description:     Initialize the keypad for use.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void Keypad_Init(void) {        // initializing the keypad
    P4->SEL0 &= ~0x7F;          // enable simple I/O
    P4->SEL1 &= ~0x7F;
    P4->DIR &= ~0x7F;           // set as input
    P4->REN |= 0x0F;            // enable internal resistor
    P4->OUT |= 0x0F;            // enable pull up resistor
}

/*--------------------------------------------------------------
 * Function:        Read_Keypad
 * Description:     Read the keys that are being pressed on the
 *                  keypad. This code was taken from the lecture
 *                  slides provided by Professor Krug.
 *
 * Inputs:          none
 *
 * Outputs:         (unsigned 8-bit int): integer returned
 *                  to main.c that tells the program if the keys
 *                  are being pressed
 *-------------------------------------------------------------*/
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

/*--------------------------------------------------------------
 * Function:        Print_Keys
 * Description:     Print the keys that were pressed from the
 *                  keypad.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
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