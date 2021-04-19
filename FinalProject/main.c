/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            04/21/2021
 * Project:         Final Project
 * File:            main.c
 * Description:     This is the final project of EGR 226. It requires the use of PWM,
 *                  TIMER A, ADC, SysTick, LCD, interrupts, DC motor, servo motor,
 *                  RGB LED, keypad, MOSFETs, optocouplers, and push buttons to make a
 *                  running state machine that will interface to the world based on
 *                  inputs from the user.
-----------------------------------------------------------------------------------------*/

// libraries
#include "msp.h"
#include "Keypad.h"
#include "LCD.h"
#include "SysTick.h"
#include "Menu.h"
#include "InitPins.h"
#include "RGB.h"
#include "Backlight.h"
#include "Interrupt.h"
#include "Passcode.h"
#include <stdlib.h>
#include <stdio.h>

// prototypes
void changeState(void);

// globals
uint8_t state;
uint8_t armed = 0;
uint8_t passFlag = 0;

void main(void) {
    __disable_irq();                                // disable interrupts
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    SysTick_Init();                                 // inits
    Pin_Init();                                     // HAS TO GO BEFORE LCD INIT
    LCD_Init();
    Keypad_Init();
    LED_init();
    DC_init();
    Button_init();
    Servo_init();
    RGB_init();
    Back_init();
    ADCsetup();
    Sounder_init();
    Touch_init();

    NVIC_EnableIRQ(PORT3_IRQn);
    NVIC_EnableIRQ(PORT6_IRQn);     // NVIC call for buttons
    __enable_irq();                 // enable interrupts

    commandWrite(0x01);     // clear screen
    delay_milli(500);       // delay 500 ms

    commandWrite(0x0C);     // turn cursor off
    delay_milli(10);        // delay 100 ms

    alarm = 0;              // setting variables to 0 to start
    buttonflag = 0;
    created = 0;
    userEnter = 0;
    state = 0;              // set first state to main menu

    while(1) {
        changeBL();         // check backlight
        changeState();      // change state
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
    uint8_t flag, push;         // local variables
    uint16_t DCduty, DCperiod;

    switch (state) {
    case 0 :            // main menu state
        if (passFlag == 0) {     // user needs to create password
            createP();
            passFlag = 1;
        }

        mainMenu();     // print to LCD
        flag = 1;

        while (flag) {
            changeBL();             // check backlight

            push = Read_Keypad();   // read keypad

            if (push) {             // if button is pushed
                if (num == 1) {     // if num is 1
                    state = 1;      // go to door menu
                    flag = 0;
                    push = 0;
                }

                if (num == 2) {     // if num is 2
                    state = 4;      // go to motor menu
                    flag = 0;
                    push = 0;
                }

                if (num == 3) {     // if num is 3
                    state = 5;      // go to lights menu
                    flag = 0;
                    push = 0;
                }

                if (num == 12) {    // if num is #
                    state = 0;      // go to main menu
                    flag = 0;
                    push = 0;
                }
            }
        }
        break;


    case 1 :            // door menu state
        doorMenu();
        flag = 1;

        while (flag) {
            changeBL();             // check backlight

            push = Read_Keypad();   // read keypad

            if (push) {
                if (num == 1) {     // if num is 1
                    if (armed == 1) {
                        TIMER_A3->CCR[1] = 750;
                        alarm = 1;
                    }
                    state = 2;      // go to open door state
                    flag = 0;
                    push = 0;
                }

                if (num == 2) {     // if num is 2
                    state = 3;      // go to close door state
                    flag = 0;
                    push = 0;
                }

                if (num == 3) {     // if num is 3
                    state = 6;      // arm / unarm state
                    flag = 0;
                    push = 0;
                }

                if (num == 12) {    // if num is #
                    state = 0;      // go to main menu
                    flag = 0;
                    push = 0;
                }
            }
        }
        break;

    case 2 :                // open door state
        P1->OUT &= ~BIT0;   // green LED on, red LED off
        P2->OUT |= BIT1;

        TIMER_A0->CCR[2] = 4500;    // move servo to open

        state = 0;
        break;

    case 3 :                // close door state
        P2->OUT &= ~BIT1;   // red LED on, green LED off
        P1->OUT |= BIT0;

        TIMER_A0->CCR[2] = 9000;    // move servo to closed

        state = 0;
        break;

    case 4 :                // motor menu state
        motorMenu();
        flag = 1;

        while (flag) {
            changeBL();     // check backlight

            push = Read_Keypad();   // read keypad

            if (push) {
                if (num <= 9) {                         // if num is 1-9
                    DCduty = DCperiod * (num * 0.1);    // set duty cycle to same %
                    flag = 0;
                    push = 0;
                }

                if (num == 10) {            // if num is *
                    DCduty = DCperiod;      // 100% duty cycle
                    flag = 0;
                    push = 0;
                }

                if (num == 11) {            // if num is 0
                    DCduty = 0;             // 0% duty cycle
                    flag = 0;
                    push = 0;
                }

                if (num == 12) {            // if num is #
                    state = 0;              // go to main menu
                    flag = 0;
                    push = 0;
                }

                TIMER_A0->CCR[4] = DCduty;      // send the duty cycle to Timer A's CCR1
            }
        }
        state = 0;
        break;

    case 5 :            // lights menu state
        lightsMenu();
        flag = 1;

        while (flag) {
            changeBL();             // check backlight

            push = Read_Keypad();   // read keypad

            if (push) {
                if (num == 1) {     // if num is 1
                    red();          // call red function
                    flag = 0;
                    push = 0;
                }

                else if (num == 2) {    // if num is 2
                    green();            // call green function
                    flag = 0;
                    push = 0;
                }

                else if (num == 3) {    // if num is 3
                    blue();             // call blue function
                    flag = 0;
                    push = 0;
                }

                else if (num == 12) {   // if num is #
                    state = 0;          // go to main menu
                    flag = 0;
                    push = 0;
                }
            }
        }
        state = 0;
        break;

    case 6 :
        if (passFlag == 1) {    // password has already been created
            password();

            if (userEnter == created) {
                armUnarm();
                flag = 1;

                while (flag) {
                    changeBL();             // check backlight

                    push = Read_Keypad();   // read keypad

                    if (push) {
                        if (num == 1) {     // if num is 1
                            armed = 1;      // arm door
                            flag = 0;
                            push = 0;
                        }

                        else if (num == 2) {    // if num is 2
                            TIMER_A3->CCR[1] = 0;
                            alarm = 0;
                            armed = 0;          // unarm door
                            flag = 0;
                            push = 0;
                        }

                        else if (num == 12) {   // if num is #
                            state = 0;          // go to main menu
                            flag = 0;
                            push = 0;
                        }
                    }
                }
            }
            state = 0;
            break;
        }
    }
}
