/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            04/04/2021
 * Project:         Lab 10 Part 2
 * File:            main_part2.c
 * Description:     This program is an extension of Part 1 of Lab 10. Instead of the
 *                  potentiometer, a temp sensor is used. Both Celcius and Fahrenheit
 *                  are printed out instead of the voltage and digital values.
-----------------------------------------------------------------------------------------*/

// libraries
#include "msp.h"
#include <stdio.h>
#include <stdlib.h>
#include "SysTick.h"

// initialization
void ADCsetup(void);

// global vars
double result = 0;
double voltage = 0;
double celc = 0;
double fahr = 0;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    SysTick_Init();                                 // initializations
    ADCsetup();

    while(1) {
        ADC14->CTL0 |= 1;                               // start conversion

        while(!ADC14->IFGR0);                           // wait until conversion is complete
        result = ADC14->MEM[5];                         // immediately store value in variable

        voltage = result * 0.0002;                      // calculations
        celc = (voltage - 0.5) / (0.01);
        fahr = (celc * (9.0/5.0) + 32);

        printf("Celcius value is: \n\t %.2lf\n", celc);       // print celcius value
        printf("Fahrenheit value is: \n\t %.2lf\n", fahr);    // print fahrenheit value

        SysTick_Delay(500);                         // delay 500 ms
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
