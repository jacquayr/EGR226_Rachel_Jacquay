/*
 * Menu.c
 *
 *  Created on: Apr 11, 2021
 *      Author: rache
 */

#include "Menu.h"
#include "LCD.h"

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
    char first[16] = "Main Menu:      ";
    char second[16] = "[1] Door Menu   ";
    char third[16] = "[2] Motor Menu  ";
    char fourth[16] = "[3] Lights Menu ";

    commandWrite(0x80);             // send cursor to first line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 16; i++) {
        dataWrite(first[i]);        // print out each letter
        delay_milli(10);            // delay 10 ms
    }

    commandWrite(0xC0);             // send cursor to second line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 16; i++) {
        dataWrite(second[i]);       // print out each letter
        delay_milli(10);            // delay 10 ms
    }

    commandWrite(0x90);             // send cursor to third line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 16; i++) {
        dataWrite(third[i]);        // print out each letter
        delay_milli(10);            // delay 10 ms
    }

    commandWrite(0xD0);             // send cursor to fourth line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 16; i++) {
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
    char first[16] = "Door Menu:      ";
    char second[16] = "[1] Open Door   ";
    char third[16] = "[2] Close Door  ";
    char fourth[16] = "                ";

    commandWrite(0x80);             // send cursor to first line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 16; i++) {
        dataWrite(first[i]);        // print out each letter
        delay_milli(10);            // delay 10 ms
    }

    commandWrite(0xC0);             // send cursor to second line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 16; i++) {
        dataWrite(second[i]);       // print out each letter
        delay_milli(10);            // delay 10 ms
    }

    commandWrite(0x90);             // send cursor to third line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 16; i++) {
        dataWrite(third[i]);        // print out each letter
        delay_milli(10);            // delay 10 ms
    }

    commandWrite(0xD0);             // send cursor to fourth line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 16; i++) {
        dataWrite(fourth[i]);       // print out each letter
        delay_milli(10);            // delay 10 ms
    }
}

/*--------------------------------------------------------------
 * Function:        motorMenu
 *
 * Description:     This function shows the motor menu screen.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void motorMenu(void) {
    int i;
    char first[16] = "Motor Menu:     ";
    char second[16] = "[0-9] 0%-90%    ";
    char third[16] = "Red Button =    ";
    char fourth[16] = "Emergency Stop  ";

    commandWrite(0x80);             // send cursor to first line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 16; i++) {
        dataWrite(first[i]);        // print out each letter
        delay_milli(10);            // delay 10 ms
    }

    commandWrite(0xC0);             // send cursor to second line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 16; i++) {
        dataWrite(second[i]);       // print out each letter
        delay_milli(10);            // delay 10 ms
    }

    commandWrite(0x90);             // send cursor to third line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 16; i++) {
        dataWrite(third[i]);        // print out each letter
        delay_milli(10);            // delay 10 ms
    }

    commandWrite(0xD0);             // send cursor to third line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 16; i++) {
        dataWrite(fourth[i]);        // print out each letter
        delay_milli(10);            // delay 10 ms
    }
}

/*--------------------------------------------------------------
 * Function:        lightsMenu
 *
 * Description:     This function shows the lights menu screen.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void lightsMenu(void) {
    int i;
    char first[16] = "Lights Menu:    ";
    char second[16] = "[1] Red         ";
    char third[16] = "[2] Green       ";
    char fourth[16] = "[3] Blue        ";

    commandWrite(0x80);             // send cursor to first line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 16; i++) {
        dataWrite(first[i]);        // print out each letter
        delay_milli(10);            // delay 10 ms
    }

    commandWrite(0xC0);             // send cursor to second line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 16; i++) {
        dataWrite(second[i]);       // print out each letter
        delay_milli(10);            // delay 10 ms
    }

    commandWrite(0x90);             // send cursor to third line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 16; i++) {
        dataWrite(third[i]);        // print out each letter
        delay_milli(10);            // delay 10 ms
    }

    commandWrite(0xD0);             // send cursor to third line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 16; i++) {
        dataWrite(fourth[i]);        // print out each letter
        delay_milli(10);            // delay 10 ms
    }
}
