/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            03/17/2021
 * Project:         Lab 7 Part 3
 * File:            main_part3.c
 * Description:     This function is similar to Part 2's code except now the line of words
 *                  is scrolling. The phrase "LABORATORY OVER" is printed onto the first
 *                  line of the LCD screen. Each second after, the phrase scrolls to the
 *                  left until the screen is blank, then it scrolls from the right side
 *                  to the left until the process starts from the beginning.
-----------------------------------------------------------------------------------------*/

#include "msp.h"
#include <stdio.h>
#include <string.h>

// function prototypes
void SysTick_Init(void);
void LCD_Init(void);
void Pin_Init(void);
void delay_micro(uint32_t microsecond);
void delay_milli(uint32_t millisecond);
void PulseEnablePin(void);
void pushNibble(uint8_t nibble);
void pushByte(uint8_t byte);
void commandWrite(uint8_t command);
void dataWrite(uint8_t data);

void print(void);
void moveLeft(void);
void rightSide(void);
void moveFromRight();

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    SysTick_Init();                                 // initializations
    Pin_Init();
    LCD_Init();

    commandWrite(0x0C);     // turn cursor off
    delay_milli(10);        // delay 10 ms

    commandWrite(0x01);     // clear screen
    delay_milli(500);       // delay 500 ms

    while (1) {
        print();            // call print function
        moveLeft();         // call move left function
        rightSide();        // call right side function
        moveFromRight();    // call move from right
    }
}

/*--------------------------------------------------------------
 * Function:        print
 *
 * Description:     This function initially prints the phrase
 *                  out on the LCD screen.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void print(void) {
    int i;
    char lab[15];
    strcpy(lab, "LABORATORY OVER");     // set lab string

    commandWrite(0x80);             // set to first line home position
    delay_micro(10);                // delay 10 ms

    for (i = 0; i < 15; i++) {
        dataWrite(lab[i]);          // write out each individual letter
        delay_milli(10);            // delay 10 ms
    }

    delay_milli(500);               // delay 500 ms for 1 sec pause
}

/*--------------------------------------------------------------
 * Function:        moveLeft
 *
 * Description:     This function moves the phrase left from the
 *                  starting spot until the screen is blank.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void moveLeft(void) {
    int i;
    char lab[15];
    char space[1];
    strcpy(lab, "LABORATORY OVER");     // set lab string
    strcpy(space, " ");                 // set space string

    for (i = 0; i < 15; i++) {      // move letters left until blank screen
        commandWrite(0x18);         // move cursor left
        delay_milli(500);           // delay 500 ms for 1 sec pause

        if (i == 7) {               // once i reaches 7, add blank space
            commandWrite(0x80);     // move to first space first line
            delay_milli(10);        // delay 10 ms
            dataWrite(space[0]);    // write blank space
            delay_milli(10);        // delay 10 ms
        }

        if (i == 8) {               // once i reaches 8, add blank space
            commandWrite(0x81);     // move to second space first line
            delay_milli(10);        // delay 10 ms
            dataWrite(space[0]);    // write blank space
            delay_milli(10);        // delay 10 ms
        }

        if (i == 9) {               // once i reaches 9, add blank space
            commandWrite(0x82);     // move to third space first line
            delay_milli(10);        // delay 10 ms
            dataWrite(space[0]);    // write blank space
            delay_milli(10);        // delay 10 ms
        }

        if (i == 10) {              // once i reaches 10, add blank space
            commandWrite(0x83);     // move to fourth space first line
            delay_milli(10);        // delay 10 ms
            dataWrite(space[0]);    // write blank space
            delay_milli(10);        // delay 10 ms
        }

        if (i == 11) {              // once i reaches 11, add blank space
            commandWrite(0x84);     // move to fifth space first line
            delay_milli(10);        // delay 10 ms
            dataWrite(space[0]);    // write blank space
            delay_milli(10);        // delay 10 ms
        }

        if (i == 12) {              // once i reaches 12, add blank space
            commandWrite(0x85);     // move to sixth space first line
            delay_milli(10);        // delay 10 ms
            dataWrite(space[0]);    // write blank space
            delay_milli(10);        // delay 10 ms
        }

        if (i == 13) {              // once i reaches 13, add blank space
            commandWrite(0x86);     // move to seventh space first line
            delay_milli(10);        // delay 10 ms
            dataWrite(space[0]);    // write blank space
            delay_milli(10);        // delay 10 ms
        }
    }

    commandWrite(0x01);             // clear screen
    delay_milli(100);               // delay 100 ms
}

/*--------------------------------------------------------------
 * Function:        rightSide
 *
 * Description:     This function moves the cursor to the right
 *                  side of the screen.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void rightSide(void) {
    int i;
    char lab[15];
    char space[1];
    strcpy(lab, "LABORATORY OVER");     // set lab string
    strcpy(space, " ");                 // set space string

    for (i = 0; i < 15; i++) {      // move cursor right
        commandWrite(0x1C);         // move cursor to the right side of screen
        delay_milli(10);            // delay 10 ms
    }

    commandWrite(0x80);             // move cursor back to first row home position
    delay_milli(100);               // delay 100 ms
}

/*--------------------------------------------------------------
 * Function:        moveFromRight
 *
 * Description:     This function moves the letters from the
 *                  right side back to the left side.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void moveFromRight(void) {
    int i;
    char lab[15];
    char space[1];
    strcpy(lab, "LABORATORY OVER");     // set lab string
    strcpy(space, " ");                 // set space string

    for (i = 0; i < 15; i++) {      // move letters left from right side
        dataWrite(lab[i]);          // print letters individually
        delay_milli(500);           // delay 500 ms
        commandWrite(0x18);         // move cursor left
        delay_milli(10);            // delay 10 ms
    }

    delay_milli(500);               // delay 500 ms for 1 sec pause
}

/*--------------------------------------------------------------
 * Function:        SysTick_Init
 * Description:     This function initializes the SysTick
 *                  component of the code.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void SysTick_Init(void) {
    SysTick->CTRL = 0;              // disable during step
    SysTick->LOAD = 0x00FFFFFF;     // max reload value
    SysTick->VAL = 0;               // any write to current value clears it
    SysTick->CTRL = 0x00000005;     // enable SysTick (3 MHz) - no interrupts
}

/*--------------------------------------------------------------
 * Function:        Pin_Init
 * Description:     This function initializes the pins on the
 *                  MSP432 that are connected to the LCD.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void Pin_Init(void) {
    // pin 3.0 = RS
    P3->SEL0 &= ~BIT0;      // set as simple I/O
    P3->SEL1 &= ~BIT0;
    P3->DIR |= BIT0;        // set as output
    P3->OUT &= ~BIT0;       // set to 0

    // pin 5.7 = E
    P5->SEL0 &= ~BIT7;      // set as simple I/O
    P5->SEL1 &= ~BIT7;
    P5->DIR |= BIT7;        // set as output
    P5->OUT &= ~BIT7;       // set to 0

    // pin 1.6 = DB4
    P1->SEL0 &= ~BIT6;      // set as simple I/O
    P1->SEL1 &= ~BIT6;
    P1->DIR |= BIT6;        // set as output
    P1->OUT &= ~BIT6;       // set to 0

    // pin 1.7 = DB5
    P1->SEL0 &= ~BIT7;      // set as simple I/O
    P1->SEL1 &= ~BIT7;
    P1->DIR |= BIT7;        // set as output
    P1->OUT &= ~BIT7;       // set to 0

    // pin 5.0 = DB6
    P5->SEL0 &= ~BIT0;      // set as simple I/O
    P5->SEL1 &= ~BIT0;
    P5->DIR |= BIT0;        // set as output
    P5->OUT &= ~BIT0;       // set to 0

    // pin 5.2 = DB7
    P5->SEL0 &= ~BIT2;      // set as simple I/O
    P5->SEL1 &= ~BIT2;
    P5->DIR |= BIT2;        // set as output
    P5->OUT &= ~BIT2;       // set to 0

    delay_milli(60);        // delay
}

/*--------------------------------------------------------------
 * Function:        delay_micro
 *
 * Description:     This function is used in the code as a
 *                  delay in microseconds to allow the MSP432
 *                  and LCD to connect correctly.
 *
 * Inputs:          (unsigned 32 bit integer) microsecond: this
 *                  variable takes in a number from the user
 *                  and converts it to a delay in microseconds
 *                  that is used in the program
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void delay_micro(uint32_t microsecond) {
    SysTick->LOAD = ((microsecond * 3) - 1);        // delay for 1 microsecond per delay value
    SysTick->VAL = 0;                               // any write to current value clears it
    while((SysTick->CTRL & 0x00010000) == 0);       // wait for flag to be set
}

/*--------------------------------------------------------------
 * Function:        delay_milli
 *
 * Description:     This function is used in the code as a
 *                  delay in milliseconds to allow the MSP432
 *                  and LCD to connect correctly.
 *
 * Inputs:          (unsigned 32 bit integer) millisecond: this
 *                  variable takes in a number from the user
 *                  and converts it to a delay in milliseconds
 *                  that is used in the program
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void delay_milli(uint32_t millisecond) {
    SysTick->LOAD = ((millisecond * 3000) - 1);           // delay for 1 millisecond per delay value
    SysTick->VAL = 0;                               // any write to current value clears it
    while ((SysTick->CTRL & 0x00010000) == 0);      // wait for flag to be set
}

/*--------------------------------------------------------------
 * Function:        PulseEnablePin
 *
 * Description:     This function enables the E pin on the LCD
 *                  which is the timer for the LCD.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void PulseEnablePin(void) {
    P5OUT &= ~BIT7;     // turn off E pin
    delay_micro(50);    // delay

    P5OUT |= BIT7;      // turn on E pin
    delay_micro(50);    // delay

    P5OUT &= ~BIT7;     // turn off E pin
    delay_micro(50);    // delay
}

/*--------------------------------------------------------------
 * Function:        pushNibble
 *
 * Description:     This function takes the nibble variable from
 *                  the function pushByte and &'s it with each
 *                  pin's respective bit, then sends Port 1 to
 *                  whichever bit the if-statement returns a 1.
 *                  After P1->OUT is done, the PulseEnablePin
 *                  function is called to set the timer.
 *
 * Inputs:          (unsigned 8 bit integer) nibble: this variable
 *                  is pushed by other functions to this function
 *                  and &'d with certain bits to check them
 *
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void pushNibble(uint8_t nibble) {
    P1OUT &= ~0x40;     // turn DB4-DB7 off
    P1OUT &= ~0x80;
    P5OUT &= ~0x01;
    P5OUT &= ~0x04;

    // DB4
    if (nibble & BIT0) {    // & nibble with BIT0
        P1->OUT |= 0x40;    // | port 1 with 0x40
    }

    // DB5
    delay_micro(50);        // delay 50 us
    if (nibble & BIT1) {    // & nibble with BIT1 to see if the same
        P1->OUT |= 0x80;    // | port 1 with 0x80
    }

    // DB6
    delay_micro(50);        // delay 50 us
    if (nibble & BIT2) {    // & nibble with BIT2
        P5->OUT |= 0x01;    // | port 1 with 0x01
    }

    // DB7
    delay_micro(50);        // delay 50 us
    if (nibble & BIT3) {    // & nibble with BIT3
        P5->OUT |= 0x04;    // | port 1 with 0x04
    }

    delay_micro(50);        // delay 50 us
    PulseEnablePin();       // call PulseEnablePin function
}

/*--------------------------------------------------------------
 * Function:        pushByte
 *
 * Description:     This function takes the byte variable from
 *                  the functions dataWrite and commandWrite
 *                  and &'s it with 0x0F, then shifts it right
 *                  4 spaces and sets it equal to the variable
 *                  nibble. From there, the function pushNibble
 *                  is called and nibble is sent.
 *
 * Inputs:          (unsigned 8 bit integer) byte: this variable
 *                  is pushed by other functions to this function
 *                  and &'d with certain bits to check them
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void pushByte(uint8_t byte) {
    uint8_t nibble;

    nibble = (byte & 0xF0) >> 4;    // &'s byte with 0xF0 then shifts right 4 spaces
    pushNibble(nibble);             // calls pushNibble with nibble
    nibble = byte & 0x0F;           // &'s byte with 0xF0 and sets nibble
    pushNibble(nibble);             // calls pushNibble with nibble
    delay_micro(100);               // delay 100 us
}

/*--------------------------------------------------------------
 * Function:        LCD_Init
 *
 * Description:     This function initializes the LCD so that the
 *                  MSP432 can communicate with it and write to it.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void LCD_Init(void) {
    commandWrite(0x3);  // sends 0x3 to the commandWrite function
    delay_milli(100);   // delays 100 ms
    commandWrite(0x3);  // sends 0x3 to the commandWrite function
    delay_milli(200);   // delays 200 ms
    commandWrite(0x3);  // sends 0x3 to the commandWrite function
    delay_milli(100);   // delays 100 ms
    commandWrite(0x2);  // sends 0x2 to the commandWrite function

    delay_milli(100);   // delays 100 ms
    commandWrite(0x28); // sends 0x28 to the commandWrite function
    delay_milli(100);   // delays 100 ms

    delay_milli(100);   // delays 100 ms
    commandWrite(0x0F); // sends 0x0F to the commandWrite function
    delay_milli(100);   // delays 100 ms
    commandWrite(0x01); // sends 0x01 to the commandWrite function - clears screen
    delay_milli(100);   // delays 100 ms
    commandWrite(0x06); // sends 0x06 to the commandWrite function
    delay_milli(10);    // delays 10 ms
}

/*--------------------------------------------------------------
 * Function:        commandWrite
 *
 * Description:     This function sets the RS pin to 0 so that it
 *                  can take in instructions and give them to
 *                  the LCD.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void commandWrite(uint8_t command) {
    P3->OUT &= ~BIT0;       // turn RS pin to instructions (0)
    pushByte(command);      // calls pushByte function
}

/*--------------------------------------------------------------
 * Function:        dataWrite
 *
 * Description:     This function sets the RS pin to 1 so that it
 *                  can take in data and give it to the LCD.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void dataWrite(uint8_t data) {
    P3->OUT |= BIT0;    // turn RS pin to data (1)
    pushByte(data);     // calls pushByte function
}
