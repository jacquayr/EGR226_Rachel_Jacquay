/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            04/21/2021
 * Project:         Final Project
 * File:            main.c
 * Description:
-----------------------------------------------------------------------------------------*/

// ADD INTERRUPTS FOR E-STOP BUTTONS !!!!!!!!!!!!!

#include "msp.h"
#include "Keypad.h"
#include "LCD.h"
#include "SysTick.h"
#include "Menu.h"
#include "InitPins.h"
#include <stdlib.h>
#include <stdio.h>

void changeState(void);

void red(void);
void blue(void);
void green(void);

uint8_t state;
volatile int DC;                // global volatile variables
volatile int period;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    SysTick_Init();                                 // inits
    Pin_Init();         // HAS TO GO BEFORE LCD INIT
    LCD_Init();
    Keypad_Init();
    LED_init();
    DC_init();
    //Servo_init();
    //RGB_init();

    commandWrite(0x01);     // clear screen
    delay_milli(500);       // delay 500 ms

    commandWrite(0x0C);     // turn cursor off
    delay_milli(10);        // delay 100 ms

    state = 0;

    while(1) {
        changeState();
    }
}

/*--------------------------------------------------------------
 * Function:        changeState
 *
 * Description:     This function changes the case based on the
 *                  count variable value.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void changeState(void) {
    uint8_t flag, push;      // local variables

    if (state == 0) {
        mainMenu();
        flag = 1;

        while (flag) {
            push = Read_Keypad();

            if (push) {
                if (num == 1) {
                    state = 1;
                    flag = 0;
                    push = 0;
                }

                else if (num == 2) {
                    state = 4;
                    flag = 0;
                    push = 0;
                }

                else if (num == 3) {
                    state = 5;
                    flag = 0;
                    push = 0;
                }
            }
        }
    }

    if (state == 1) {
        doorMenu();
        flag = 1;

        while (flag) {
            push = Read_Keypad();

            if (push) {
                if (num == 1) {         // open door
                    state = 2;
                    flag = 0;
                }

                if (num == 2) {    // close door
                    state = 3;
                    flag = 0;
                }
            }
        }
    }

    if (state == 2) {
        P1->OUT &= ~BIT0;   // green LED on, red LED off
        P2->OUT |= BIT1;

        // add in opening the door function !!!!!!!!!

        SysTick_Delay(500);
        state = 0;
    }

    if (state == 3) {
        P2->OUT &= ~BIT1;   // red LED on, green LED off
        P1->OUT |= BIT0;

        // add in closing the door function !!!!!!!!!

        SysTick_Delay(500);
        state = 0;
    }

    if (state == 4) {
        motorMenu();

        flag = 1;

        while (flag) {
            push = Read_Keypad();

            if (push) {
                if (num <= 9) {
                    DC = period * (num * 0.1);      // set duty cycle to match the intensity of whichever button was pressed
                    flag = 0;
                    push = 0;
                }

                if (num == 10) {
                    DC = period * 0.99;
                    flag = 0;
                    push = 0;
                }

                if (num == 11) {
                    DC = 0;
                    flag = 0;
                    push = 0;
                }

                TIMER_A0->CCR[4] = DC;              // send the duty cycle to Timer A's CCR1
            }

            state = 0;
        }
    }

    if (state == 5) {
        lightsMenu();
    }
}
