/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            04/21/2021
 * Project:         Final Project
 * File:            main.c
 * Description:
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
#include <stdlib.h>
#include <stdio.h>

// prototypes
void changeState(void);
void PORT6_IRQHandler(void);
void PORT3_IRQHandler(void);
void debounce(void);
void password(void);
void createP(void);

// globals
uint8_t state, buttonflag;
uint8_t armed = 0;
uint16_t created = 0;
uint16_t userEnter = 0;
uint8_t passFlag = 0;
uint8_t alarm = 0;

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

        TIMER_A0->CCR[2] = 3000;    // move servo to open

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
            buttonflag = 1;                                                // set button flag to 1
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

        while (buttonflag == 1) {                   // do while button flag is set from debounce function
            changeBL();

            if ((P6->IN & BIT4) != BIT4) {          // if red button is pressed
                TIMER_A0->CCR[4] = 0;               // set duty cycle
            }

            if ((P6->IN & BIT5) != BIT5) {              // if blue button is pressed
                if (redOn == 0) {                       // if red flag says that the light is off
                    TIMER_A2->CCR[1] = 300 * redSum;    // turn light on to the past duty cycle
                    redOn = 1;                          // set red flag to on
                }

                else if (redOn == 1) {                  // if red flag says that light is on
                    TIMER_A2->CCR[1] = 0;               // turn light off
                    redOn = 0;                          // set red flag to off
                }

                if (greenOn == 0) {                     // if green flag says that light is off
                    TIMER_A0->CCR[3] = 300 * greenSum;  // turn light on to the past duty cycle
                    greenOn = 1;                        // set green flag to on
                }

                else if (greenOn == 1) {                // if green flag says that light is on
                    TIMER_A0->CCR[3] = 0;               // turn light off
                    greenOn = 0;                        // set green flag to off
                }

                if (blueOn == 0) {                      // if blue flag says that light is off
                    TIMER_A2->CCR[3] = 300 * blueSum;   // turn light on to the past duty cycle
                    blueOn = 1;                         // set blue flag to on
                }

                else if (blueOn == 1) {                 // if blue flag says that light is on
                    TIMER_A2->CCR[3] = 0;               // turn light off
                    blueOn = 0;                         // set blue flag to off
                }
            }

            buttonflag = 0;       // reset button flag to 0
        }
    }

    P6->IFG &= ~0x30;       // reset interrupt flag
}

/*--------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void password(void) {
    int p1, p2, p3, sum, push, flag;
    p1 = 0;
    p2 = 0;
    p3 = 0;
    sum = 0;
    push = 0;
    flag = 0;

    passcode();         // tell user to press 3 buttons (000-100)
    flag = 1;           // read keypad 3x

    while (flag) {
        changeBL();

        push = Read_Keypad();

        if (push) {                                     // if key is pressed
            if (num == 10 | num == 11 | num == 12) {
                num = 0;
            }
            p1 = num;
            push = 0;
            flag = 0;
        }
    }

    flag = 1;

    while (flag) {
        changeBL();

        push = Read_Keypad();

        if (push) {                                     // if key is pressed
            if (num == 10 | num == 11 | num == 12) {
                num = 0;
            }
            p2 = num;
            push = 0;
            flag = 0;
        }
    }

    flag = 1;

    while (flag) {
        changeBL();

        push = Read_Keypad();

        if (push) {                                     // if key is pressed
            if (num == 10 | num == 11 | num == 12) {
                num = 0;
            }
            p3 = num;
            push = 0;
            flag = 0;
        }
    }

    p1 *= 100;
    p2 *= 10;
    sum = p1 + p2 + p3;

    userEnter = sum;
}

/*--------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void createP(void) {
    int p1, p2, p3, sum, push, flag;
    p1 = 0;
    p2 = 0;
    p3 = 0;
    sum = 0;
    push = 0;
    flag = 0;

    firstPasscode();         // tell user to press 3 buttons (000-100)
    flag = 1;           // read keypad 3x

    while (flag) {
        changeBL();

        push = Read_Keypad();

        if (push) {                                     // if key is pressed
            if (num == 10 | num == 11 | num == 12) {
                num = 0;
            }
            p1 = num;
            push = 0;
            flag = 0;
        }
    }

    flag = 1;

    while (flag) {
        changeBL();

        push = Read_Keypad();

        if (push) {                                     // if key is pressed
            if (num == 10 | num == 11 | num == 12) {
                num = 0;
            }
            p2 = num;
            push = 0;
            flag = 0;
        }
    }

    flag = 1;

    while (flag) {
        changeBL();

        push = Read_Keypad();

        if (push) {                                     // if key is pressed
            if (num == 10 | num == 11 | num == 12) {
                num = 0;
            }
            p3 = num;
            push = 0;
            flag = 0;
        }
    }

    p1 *= 100;
    p2 *= 10;
    sum = p1 + p2 + p3;

    created = sum;
}

/*--------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void PORT3_IRQHandler(void) {
    if (P3->IFG & BIT7) {       // if port 6 interrupts were changed and the flag was set
        TIMER_A3->CCR[0] = 600;
        TIMER_A3->CCR[1] = 300;
        SysTick_Delay(200);
        TIMER_A3->CCR[1] = 0;
        SysTick_Delay(100);

        TIMER_A3->CCR[0] = 800;
        TIMER_A3->CCR[1] = 400;
        SysTick_Delay(400);
        TIMER_A3->CCR[1] = 0;

        TIMER_A3->CCR[0] = 1500;

        if (alarm == 1) {
            TIMER_A3->CCR[1] = 750;
        }
    }

    P3->IFG &= ~BIT7;       // reset interrupt flag
}
