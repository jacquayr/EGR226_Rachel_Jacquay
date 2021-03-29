/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            04/04/2021
 * Project:         Lab 10 Extra Credit
 * File:            main_EC.c
 * Description:     This program is an extension of Part 3 of Lab 10, except a pushbutton
 *                  was added into the circuit. A press changed the temperature being
 *                  printed out from Celcius to Fahrenheit. GPIO interrupts were used.
-----------------------------------------------------------------------------------------*/

// libraries
#include "msp.h"
#include <stdio.h>
#include <stdlib.h>
#include "LCD.h"
#include <string.h>

// initializations
void ADCsetup(void);
void firstline(void);
void secondline(void);
void debounce(void);
void printCelc(void);
void printFahr(void);
void PORT6_IRQHandler(void);

// globals
double result = 0;
double voltage = 0;
double celc = 0;
double fahr = 0;
int flag = 0;
int i = 0;

void main(void)
{
    __disable_irq();                                // disable irq
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    SysTick_Init();                                 // initializations
    ADCsetup();
    Pin_Init();
    LCD_Init();

    NVIC_EnableIRQ(PORT6_IRQn);     // NVIC call
    __enable_irq();                 // enable irq

    commandWrite(0x01);     // clear screen
    delay_milli(500);       // delay 500 ms

    commandWrite(0x0C);     // turn cursor off
    delay_milli(10);        // delay 100 ms

    firstline();            // call first line function
    delay_micro(50);        // delay 50 us

    while(1) {
        ADC14->CTL0 |= 1;                       // start conversion

        while(!ADC14->IFGR0);                   // wait until conversion is complete
        result = ADC14->MEM[5];                 // immediately store value in variable

        voltage = result * 0.0002;              // find voltage
        celc = (voltage - 0.5) / (0.01);        // find celcius value
        fahr = (celc * (9.0/5.0) + 32);         // find fahr value

        if (flag == 0) {            // if flag is 0 (default)
            printCelc();            // print celc
        }

        else if (flag == 1) {       // if flag is 1 (first push)
            printFahr();            // print fahr
        }

        delay_milli(500);                       // delay 500 ms
    }
}

/*--------------------------------------------------------------
 * Function:        ADCsetup
 *
 * Description:     This function sets up the pin 5.5 for ADC
 *                  and also sets up the ADC in the MSP432 for
 *                  use.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void ADCsetup(void) {
    ADC14->CTL0 = 0x00000010;       // power on & disabled during config
    ADC14->CTL0 |= 0x04D80300;      // S/H pulse mode, MCLK, 32 sample clocks, SW trigger, /4 clock divide

    ADC14->CTL1 = 0x00000030;       // 14-bit resolution
    ADC14->MCTL[5] = 0;             // A0 input, single ended, vref = avcc

    P5->SEL1 |= BIT5;               // configure P5.5 for AO
    P5->SEL0 |= BIT5;

    ADC14->CTL1 |= 0x00050000;      // start converting at mem reg 5
    ADC14->CTL0 |= 2;               // enable ADC after config
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
    if ((P6->IN & BIT4) != BIT4) {          // if switch is pressed
        delay_milli(10);                    // delay for 10 ms

        if ((P6->IN & BIT4) != BIT4) {      // check switch again
            flag++;                         // increment flag

            if (flag == 2) {    // if flag is 2
                flag = 0;       // reset to 0
            }
        }
    }
}

/*--------------------------------------------------------------
 * Function:        firstline
 *
 * Description:     This function sets the cursor to the first
 *                  line and then prints out the current temp
 *                  phrase.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void firstline(void) {
    int i;
    char line1[15];
    strcpy(line1, "Current temp is");   // copy temp to line1 var

    commandWrite(0x80);                 // send cursor to first line
    delay_milli(100);                   // delay 100 ms

    for (i = 0; i < 15; i++) {
        dataWrite(line1[i]);            // write each character of the string
        delay_milli(10);                // delay 10 ms
    }
}

/*--------------------------------------------------------------
 * Function:        printCelc
 *
 * Description:     This function sets the cursor to the second
 *                  line and then prints out the current temp
 *                  as well as the degree symbol and the letter
 *                  C.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void printCelc(void) {
    int i;
    char celcius[4];
    sprintf(celcius, "%.2lf", celc);   // copy celcius valur to temp string w/ 2 decmial places

    commandWrite(0xC5);             // send cursor to middle of second line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 4; i++) {
        dataWrite(celcius[i]);         // write each character of the temperature string
        delay_milli(50);            // delay 50 ms
    }

    dataWrite(0xDF);        // write the degree symbol
    delay_milli(10);        // delay 10 ms

    dataWrite(0x43);        // write letter C
    delay_milli(10);        // delay 10 ms
}

/*--------------------------------------------------------------
 * Function:        printFahr
 *
 * Description:     This function sets the cursor to the second
 *                  line and then prints out the current temp
 *                  as well as the degree symbol and the letter
 *                  F.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void printFahr(void) {
    int i;
    char fahren[4];
    sprintf(fahren, "%.2lf", fahr);   // copy celcius valur to temp string w/ 2 decmial places

    commandWrite(0xC5);             // send cursor to middle of second line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 4; i++) {
        dataWrite(fahren[i]);       // write each character of the temperature string
        delay_milli(50);            // delay 50 ms
    }

    dataWrite(0xDF);        // write the degree symbol
    delay_milli(10);        // delay 10 ms

    dataWrite(0x46);        // write letter F
    delay_milli(10);        // delay 10 ms
}

/*--------------------------------------------------------------
 * Function:        PORT6_IRQHandler
 *
 * Description:     This function deals with the GPIO interrupt
 *                  and calls the debounce function, then checks
 *                  if the button is pressed, and acts
 *                  accordingly.
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void PORT6_IRQHandler(void) {
    // button: P6.4

    if (P6->IFG & BIT4) {       // if port 6 interrupts were changed and the flag was set
        debounce();             // call debounce function

        P6->IFG &= ~BIT4;       // reset interrupt flag
    }
}