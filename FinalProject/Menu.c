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
    char fourth[16] = "[3] Arm / Unarm ";

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
 * Function:        armDoor
 *
 * Description:     This function shows the arm door menu screen.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void armUnarm(void) {
    int i;
    char first[16] = "Arm / Unarm:    ";
    char second[16] = "[1] Arm Door    ";
    char third[16] = "[2] Unarm Door  ";
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
    char third[16] = "[*] 100%        ";
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

    commandWrite(0xD0);             // send cursor to fourth line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 16; i++) {
        dataWrite(fourth[i]);        // print out each letter
        delay_milli(10);            // delay 10 ms
    }
}

/*--------------------------------------------------------------
 * Function:        redMenu
 *
 * Description:     This function shows the red LED menu.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void redMenu(void) {
    int i;
    char first[16] = "Red Menu:       ";
    char second[16] = "Enter 3 digits  ";
    char third[16] = "                ";
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
 * Function:        blueMenu
 *
 * Description:     This function shows the blue LED menu.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void blueMenu(void) {
    int i;
    char first[16] = "Blue Menu:      ";
    char second[16] = "Enter 3 digits  ";
    char third[16] = "                ";
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
 * Function:        greenMenu
 *
 * Description:     This function shows the green LED menu.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void greenMenu(void) {
    int i;
    char first[16] = "Green Menu:     ";
    char second[16] = "Enter 3 digits  ";
    char third[16] = "                ";
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
 * Function:        passcode
 *
 * Description:     This function tells the user to enter a
 *                  3-digit passcode.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void passcode(void) {
    int i;
    char first[16] = "Enter 3-digit   ";
    char second[16] = "Passcode        ";
    char third[16] = "                ";
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
 * Function:        passcode
 *
 * Description:     This function tells the user to enter a
 *                  3-digit passcode at the beginning to create
 *                  one.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void firstPasscode(void) {
    int i;
    char first[16] = "Please create a ";
    char second[16] = "3-digit password";
    char third[16] = "to arm / unarm  ";
    char fourth[16] = "the door        ";

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
