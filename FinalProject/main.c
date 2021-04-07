/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            04/21/2021
 * Project:         Final Project
 * File:            main.c
 * Description:
-----------------------------------------------------------------------------------------*/

// ADD PORT 4 INTERRUPTS FOR KEYPAD !!!!!!!!!!!!!

#include "msp.h"
#include "Keypad.h"
#include "LCD.h"
#include "SysTick.h"
#include <stdlib.h>
#include <stdio.h>

void changeCase(void);
void mainMenu(void);
void doorMenu(void);
void openClose(void);
void LED_init(void);

uint8_t count = 0;
uint8_t closeOpen = 0;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    SysTick_Init();
    Pin_Init();         // HAS TO GO BEFORE LCD INIT
    LCD_Init();
    Keypad_Init();
    LED_init();

    commandWrite(0x01);     // clear screen
    delay_milli(500);       // delay 500 ms

    commandWrite(0x0C);     // turn cursor off
    delay_milli(10);        // delay 100 ms

    mainMenu();

    while(1) {
        pressed = Read_Keypad();                    // call keypad read function and output to pressed
        if (pressed) {                              // if a 1 is returned
            Print_Keys();                           // call print keys function with num variable
            SysTick_Delay(100);                     // delay

            count = num;
            changeCase();
        }
    }
}

/*--------------------------------------------------------------
 * Function:        changeCase
 *
 * Description:     This function changes the case based on the
 *                  count variable value.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void changeCase(void) {
    switch (count) {        // switch statement
    case 0 :
        mainMenu();
        break;              // break

    case 1 :
        doorMenu();
        pressed = Read_Keypad();                    // call keypad read function and output to pressed
        if (pressed) {                              // if a 1 is returned
            Print_Keys();                           // call print keys function with num variable
            SysTick_Delay(100);                     // delay

            closeOpen = num;
        }
        openClose();
        break;

    default :               // default case
        mainMenu();
        break;
    }
}

/*--------------------------------------------------------------
 * Function:        mainMenu
 *
 * Description:     This function shows the main menu screen.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void mainMenu(void) {
    int i;
    char first[10] = "Main Menu:";
    char second[13] = "[1] Door Menu";
    char third[14] = "[2] Motor Menu";
    char fourth[15] = "[3] Lights Menu";

    commandWrite(0x80);             // send cursor to first line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 10; i++) {
        dataWrite(first[i]);        // print out each letter
        delay_milli(10);            // delay 10 ms
    }

    commandWrite(0xC0);             // send cursor to second line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 13; i++) {
        dataWrite(second[i]);       // print out each letter
        delay_milli(10);            // delay 10 ms
    }

    commandWrite(0x90);             // send cursor to third line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 14; i++) {
        dataWrite(third[i]);        // print out each letter
        delay_milli(10);            // delay 10 ms
    }

    commandWrite(0xD0);             // send cursor to fourth line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 15; i++) {
        dataWrite(fourth[i]);       // print out each letter
        delay_milli(10);            // delay 10 ms
    }
}

/*--------------------------------------------------------------
 * Function:        doorMenu
 *
 * Description:     This function shows the door menu screen.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void doorMenu(void) {
    int i;
    char first[10] = "Door Menu:";
    char second[13] = "[1] Open Door";
    char third[14] = "[2] Close Door";

    commandWrite(0x01);     // clear screen
    delay_milli(500);       // delay 500 ms

    commandWrite(0x80);             // send cursor to first line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 10; i++) {
        dataWrite(first[i]);        // print out each letter
        delay_milli(10);            // delay 10 ms
    }

    commandWrite(0xC0);             // send cursor to second line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 13; i++) {
        dataWrite(second[i]);       // print out each letter
        delay_milli(10);            // delay 10 ms
    }

    commandWrite(0x90);             // send cursor to third line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 14; i++) {
        dataWrite(third[i]);        // print out each letter
        delay_milli(10);            // delay 10 ms
    }
}

/*--------------------------------------------------------------
 * Function:        openClose
 *
 * Description:     This function turns the LEDs for the door
 *                  on or off based on which selection they made.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void openClose(void) {
    if (closeOpen == 1) {
        P1->OUT &= ~BIT0;
        P2->OUT |= BIT1;
    }

    else if (closeOpen == 2) {
        P2->OUT &= ~BIT1;
        P1->OUT |= BIT0;
    }
}

/*--------------------------------------------------------------
 * Function:        LED_init
 *
 * Description:     This function initializes the red and green
 *                  LEDs for the door.
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
    P1->OUT |= BIT0;

    // green LED = P2.1
    P2->SEL0 &= ~BIT1;
    P2->SEL1 &= ~BIT1;
    P2->DIR |= BIT1;
    P2->OUT &= ~BIT1;
}
