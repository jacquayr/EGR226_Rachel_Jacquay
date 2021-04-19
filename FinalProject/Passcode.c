/*
 * Passcode.c
 *
 *  Created on: Apr 15, 2021
 *      Author: rache
 */

#include "Passcode.h"
#include "Backlight.h"
#include "Keypad.h"

/*--------------------------------------------------------------
 * Function:        password
 *
 * Description:     This function takes in the three pushes given
 *                  by the user and sets the new entry equal to
 *                  the sum of the pushes.
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
        changeBL();     // update backlight

        push = Read_Keypad();       // read keypad

        if (push) {                                     // if key is pressed
            if (num == 10 | num == 11 | num == 12) {    // if num pressed is * or 0 or #
                num = 0;                                // num is 0
            }
            p1 = num;       // set press equal to num
            push = 0;
            flag = 0;
        }
    }

    flag = 1;

    while (flag) {
        changeBL();     // update backlight

        push = Read_Keypad();       // read keypad

        if (push) {                                     // if key is pressed
            if (num == 10 | num == 11 | num == 12) {    // if num pressed is * or 0 or #
                num = 0;                                // num is 0
            }
            p2 = num;       // set press equal to num
            push = 0;
            flag = 0;
        }
    }

    flag = 1;

    while (flag) {
        changeBL();     // update backlight

        push = Read_Keypad();       // read keypad

        if (push) {                                     // if key is pressed
            if (num == 10 | num == 11 | num == 12) {    // if num pressed is * or 0 or #
                num = 0;                                // num is 0
            }
            p3 = num;       // set press equal to num
            push = 0;
            flag = 0;
        }
    }

    p1 *= 100;
    p2 *= 10;
    sum = p1 + p2 + p3;     // create a sum to be compared later

    userEnter = sum;        // set current entry equal to sum
}

/*--------------------------------------------------------------
 * Function:        createP
 *
 * Description:     This function allows the user to create a
 *                  3-digit password at the start up of the
 *                  code.
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
        changeBL();     // update backlight

        push = Read_Keypad();       // read keypad

        if (push) {                                     // if key is pressed
            if (num == 10 | num == 11 | num == 12) {    // if num pressed is * or 0 or #
                num = 0;                                // num is 0
            }
            p1 = num;       // set press equal to num
            push = 0;
            flag = 0;
        }
    }

    flag = 1;

    while (flag) {
        changeBL();     // update backlight

        push = Read_Keypad();       // read keypad

        if (push) {                                     // if key is pressed
            if (num == 10 | num == 11 | num == 12) {    // if num pressed is * or 0 or #
                num = 0;                                // num is 0
            }
            p2 = num;       // set press equal to num
            push = 0;
            flag = 0;
        }
    }

    flag = 1;

    while (flag) {
        changeBL();     // update backlight

        push = Read_Keypad();       // read keypad

        if (push) {                                     // if key is pressed
            if (num == 10 | num == 11 | num == 12) {    // if num pressed is * or 0 or #
                num = 0;                                // num is 0
            }
            p3 = num;       // set press equal to num
            push = 0;
            flag = 0;
        }
    }

    p1 *= 100;
    p2 *= 10;
    sum = p1 + p2 + p3;     // create a sum to be compared later

    created = sum;        // set original password equal to sum
}
