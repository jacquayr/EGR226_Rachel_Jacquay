/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            04/04/2021
 * Project:         Lab 10 Part 3
 * File:            main_part3.c
 * Description:     This program is an extension of Part 2 of Lab 10. It takes the temp
 *                  from the temperature sensor, and prints it out in Celcius onto the
 *                  LCD instead of the CCS window. The temp printed changes every 500ms.
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

// globals
double result = 0;
double voltage = 0;
double celc = 0;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    SysTick_Init();                                 // initializations
    ADCsetup();
    Pin_Init();
    LCD_Init();

    commandWrite(0x01);     // clear screen
    delay_milli(500);       // delay 500 ms

    commandWrite(0x0C);     // turn cursor off
    delay_milli(100);       // delay 100 ms

    firstline();            // call first line function
    delay_micro(50);        // delay 50 us

    while(1) {
        ADC14->CTL0 |= 1;                               // start conversion

        while(!ADC14->IFGR0);                           // wait until conversion is complete
        result = ADC14->MEM[5];                         // immediately store value in variable

        voltage = result * 0.0002;              // calculations
        celc = (voltage - 0.5) / (0.01);        // find celcius value

        secondline();                           // call second line function
        delay_micro(50);                        // delay 50 us

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
 * Function:        secondline
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
void secondline(void) {
    int i;
    char temp[4];
    sprintf(temp, "%.2lf", celc);   // copy celcius valur to temp string w/ 2 decmial places

    commandWrite(0xC5);             // send cursor to middle of second line
    delay_milli(10);                // delay 10 ms

    for (i = 0; i < 4; i++) {
        dataWrite(temp[i]);         // write each character of the temperature string
        delay_milli(50);            // delay 50 ms
    }

    dataWrite(0xDF);        // write the degree symbol
    delay_milli(10);        // delay 10 ms

    dataWrite(0x43);        // write letter C
    delay_milli(10);        // delay 10 ms
}