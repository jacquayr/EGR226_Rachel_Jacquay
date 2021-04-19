/*
 * Interrupt.c
 *
 *  Created on: Apr 15, 2021
 *      Author: rache
 */

#include "Interrupt.h"
#include "SysTick.h"
#include "RGB.h"
#include "Backlight.h"

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
 * Function:        PORT3_IRQHandler
 *
 * Description:     This function deals with the GPIO interrupt
 *                  that works with the touch sensor.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void PORT3_IRQHandler(void) {
    if (P3->IFG & BIT7) {       // if port 3 interrupts were changed and the flag was set
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
    if (((P6->IN & BIT4) != BIT4) || ((P6->IN & BIT5) != BIT5)) {          // if any button is pressed
        SysTick_Delay(10);                                                 // delay for 10 ms

        if (((P6->IN & BIT4) != BIT4) || ((P6->IN & BIT5) != BIT5)) {      // check button again
            buttonflag = 1;                                                // set button flag to 1
        }
    }
}
