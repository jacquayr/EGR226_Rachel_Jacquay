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