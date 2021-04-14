/*
 * RGB.c
 *
 *  Created on: Apr 12, 2021
 *      Author: rache
 */

#include "RGB.h"
#include "Keypad.h"
#include "Menu.h"

/*--------------------------------------------------------------
 * Function:        red
 *
 * Description:     This function reads 3 presses from the user
 *                  and determines the brightness of the red
 *                  LED, then turns it on at that amount.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void red(void) {
    int p1, p2, p3, sum, push, flag;
    p1 = 0;
    p2 = 0;
    p3 = 0;
    sum = 0;
    push = 0;
    flag = 0;
    redSum = 0;

    redMenu();          // tell user to press 3 buttons (000-100)
    flag = 1;           // read keypad 3x

    while (flag) {
        push = Read_Keypad();

        if (push) {
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
        push = Read_Keypad();

        if (push) {
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
        push = Read_Keypad();

        if (push) {
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

    if (sum >= 100) {
        sum = 100;
    }

    redSum = sum;

    if (redSum != 0) {      // if DC is not zero
        redOn = 1;          // tell user that red is ON
    }

    if (redSum == 0) {      // if DC is zero
        redOn = 0;          // tell user that red is OFF
    }

    TIMER_A2->CCR[1] = 300 * sum;                         // set duty cycle
}

/*--------------------------------------------------------------
 * Function:        green
 *
 * Description:     This function reads 3 presses from the user
 *                  and determines the brightness of the green
 *                  LED, then turns it on at that amount.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void green(void) {
    int p1, p2, p3, sum, push, flag;
    p1 = 0;
    p2 = 0;
    p3 = 0;
    sum = 0;
    push = 0;
    flag = 0;

    greenMenu();        // tell user to press 3 buttons (000-100)
    flag = 1;           // read keypad 3x

    while (flag) {
        push = Read_Keypad();

        if (push) {
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
        push = Read_Keypad();

        if (push) {
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
        push = Read_Keypad();

        if (push) {
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

    if (sum >= 100) {
        sum = 100;
    }

    greenSum = sum;

    if (greenSum != 0) {      // if DC is not zero
        greenOn = 1;          // tell user that red is ON
    }

    if (greenSum == 0) {      // if DC is zero
        greenOn = 0;          // tell user that red is OFF
    }

    TIMER_A0->CCR[3] = 300 * sum;                         // set duty cycle
}

/*--------------------------------------------------------------
 * Function:        blue
 *
 * Description:     This function reads 3 presses from the user
 *                  and determines the brightness of the blue
 *                  LED, then turns it on at that amount.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void blue(void) {
    int p1, p2, p3, sum, push, flag;
    p1 = 0;
    p2 = 0;
    p3 = 0;
    sum = 0;
    push = 0;
    flag = 0;

    blueMenu();         // tell user to press 3 buttons (000-100)
    flag = 1;           // read keypad 3x

    while (flag) {
        push = Read_Keypad();

        if (push) {
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
        push = Read_Keypad();

        if (push) {
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
        push = Read_Keypad();

        if (push) {
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

    if (sum >= 100) {
        sum = 100;
    }

    blueSum = sum;

    if (blueSum != 0) {      // if DC is not zero
        blueOn = 1;          // tell user that red is ON
    }

    if (blueSum == 0) {      // if DC is zero
        blueOn = 0;          // tell user that red is OFF
    }

    TIMER_A2->CCR[3] = 300 * sum;                         // set duty cycle
}
