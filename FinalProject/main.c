/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            04/21/2021
 * Project:         Final Project
 * File:            main.c
 * Description:
-----------------------------------------------------------------------------------------*/

#include "msp.h"
#include "Keypad.h"
#include "LCD.h"
#include "SysTick.h"
#include "Menu.h"
#include "InitPins.h"
#include "RGB.h"

#include <stdlib.h>
#include <stdio.h>

void changeState(void);
void PORT6_IRQHandler(void);
void debounce(void);
void changeBL(void);

uint8_t state, buttonflag;

void main(void)
{
    __disable_irq();
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    SysTick_Init();                                 // inits
    Pin_Init();         // HAS TO GO BEFORE LCD INIT
    LCD_Init();
    Keypad_Init();
    LED_init();
    DC_init();
    Button_init();
    Servo_init();
    RGB_init();
    Back_init();
    ADCsetup();

    NVIC_EnableIRQ(PORT6_IRQn);                     // NVIC call for buttons
    __enable_irq();

    commandWrite(0x01);     // clear screen
    delay_milli(500);       // delay 500 ms

    commandWrite(0x0C);     // turn cursor off
    delay_milli(10);        // delay 100 ms

    state = 0;

    while(1) {
        changeBL();
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
    uint16_t DCduty, DCperiod;

    switch (state) {
    case 0 :
        mainMenu();
        flag = 1;

        while (flag) {
            changeBL();

            push = Read_Keypad();

            if (push) {
                if (num == 1) {
                    state = 1;
                    flag = 0;
                    push = 0;
                }

                if (num == 2) {
                    state = 4;
                    flag = 0;
                    push = 0;
                }

                if (num == 3) {
                    state = 5;
                    flag = 0;
                    push = 0;
                }

                if (num == 12) {
                    state = 0;
                    flag = 0;
                    push = 0;
                }
            }
        }
        break;


    case 1 :
        doorMenu();
        flag = 1;

        while (flag) {
            changeBL();

            push = Read_Keypad();

            if (push) {
                if (num == 1) {    // open door
                    state = 2;
                    flag = 0;
                    push = 0;
                }

                if (num == 2) {    // close door
                    state = 3;
                    flag = 0;
                    push = 0;
                }

                if (num == 12) {
                    state = 0;
                    flag = 0;
                    push = 0;
                }
            }
        }
        break;

    case 2 :
        P1->OUT &= ~BIT0;   // green LED on, red LED off
        P2->OUT |= BIT1;

        TIMER_A0->CCR[2] = 3000;

        SysTick_Delay(500);
        state = 0;
        break;

    case 3 :
        P2->OUT &= ~BIT1;   // red LED on, green LED off
        P1->OUT |= BIT0;

        TIMER_A0->CCR[2] = 9000;

        SysTick_Delay(500);
        state = 0;
        break;

    case 4 :
        motorMenu();
        flag = 1;

        while (flag) {
            changeBL();

            push = Read_Keypad();

            if (push) {
                if (num <= 9) {
                    DCduty = DCperiod * (num * 0.1);
                    flag = 0;
                    push = 0;
                }

                if (num == 10) {
                    DCduty = DCperiod;
                    flag = 0;
                    push = 0;
                }

                if (num == 11) {
                    DCduty = 0;
                    flag = 0;
                    push = 0;
                }

                if (num == 12) {
                    state = 0;
                    flag = 0;
                    push = 0;
                }

                TIMER_A0->CCR[4] = DCduty;              // send the duty cycle to Timer A's CCR1
            }
        }
        state = 0;
        break;

    case 5 :
        lightsMenu();
        flag = 1;

        while (flag) {
            changeBL();

            push = Read_Keypad();

            if (push) {
                if (num == 1) {
                    red();
                    flag = 0;
                    push = 0;
                }

                else if (num == 2) {
                    green();
                    flag = 0;
                    push = 0;
                }

                else if (num == 3) {
                    blue();
                    flag = 0;
                    push = 0;
                }

                else if (num == 12) {
                    state = 0;
                    flag = 0;
                    push = 0;
                }
            }
        }
        state = 0;
        break;
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
            buttonflag = 1;                                                      // set button flag to 1
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

        while (buttonflag == 1) {                         // do while button flag is set from debounce function
            changeBL();

            if ((P6->IN & BIT4) != BIT4) {          // if red button is pressed
                TIMER_A0->CCR[4] = 0;               // set duty cycle
            }

            if ((P6->IN & BIT5) != BIT5) {          // if blue button is pressed
                if (redOn == 0) {
                    TIMER_A2->CCR[1] = 300 * redSum;
                    redOn = 1;
                }

                else if (redOn == 1) {
                    TIMER_A2->CCR[1] = 0;
                    redOn = 0;
                }

                if (greenOn == 0) {
                    TIMER_A0->CCR[3] = 300 * greenSum;
                    greenOn = 1;
                }

                else if (greenOn == 1) {
                    TIMER_A0->CCR[3] = 0;
                    greenOn = 0;
                }

                if (blueOn == 0) {
                    TIMER_A2->CCR[3] = 300 * blueSum;
                    blueOn = 1;
                }

                else if (blueOn == 1) {
                    TIMER_A2->CCR[3] = 0;
                    blueOn = 0;
                }
            }

            buttonflag = 0;       // reset button flag to 0
        }
    }

    P6->IFG &= ~0x30;       // reset interrupt flag
}

/*--------------------------------------------------------------
 * Function:        changeBL
 *
 * Description:     This function changes the backlight of the
 *                  LCD.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void changeBL(void) {
    float result = 0.0;

    ADC14->CTL0 |= 1;               // start conversion
    while(!ADC14->IFGR0);           // wait until conversion is complete
    result = ADC14->MEM[5];         // immediately store value in variable
    TIMER_A1->CCR[1] = result * 0.0609;                               // set duty cycle
}
