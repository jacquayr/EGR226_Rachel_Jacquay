/*-----------------------------------------------------------------------------------------
 * Author:          Rachel Jacquay
 * Course:          EGR 226-902
 * Date:            04/21/2021
 * Project:         Final Project Testing
 * File:            main.c
 * Description:
-----------------------------------------------------------------------------------------*/

// libraries
#include "msp.h"
#include "Keypad.h"
#include "LCD.h"
#include "SysTick.h"
#include "Menu.h"
#include "InitPins.h"
#include "RGB.h"

#include <stdio.h>
#include <stdlib.h>

// function prototypes
void T32_INT2_IRQHandler(void);
void ADCsetup(void);

volatile float result = 0.0;
volatile float voltage = 0.0;

void main(void)
{
    __disable_irq();
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    ADCsetup();
    Back_init();

    P2->SEL0 &= ~BIT2;      // blue LED on MSP
    P2->SEL1 &= ~BIT2;
    P2->DIR |= BIT2;
    P2->OUT |= BIT2;

    TIMER32_2->CONTROL = 0b11100011;    // TIMER 2 for Enabled, Periodic, 2/ Interrupt, No Prescale, 32 bit, One Shot
    // See page 589 in reference manual

    NVIC_EnableIRQ(T32_INT2_IRQn);
    __enable_irq();

    TIMER32_2->LOAD = 50000;      // load 3sec

    while(1) {

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
 * Function:        T32_INT2_IRQHandler
 *
 * Description:
 *
 * Inputs:          none
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void T32_INT2_IRQHandler(void) {
    // pot = P7.7
    // mosfet = P8.4

    TIMER32_2->INTCLR = 1;      // Clear interrupt flag so it does not interrupt again immediately

    ADC14->CTL0 |= 1;               // start conversion
    while(!ADC14->IFGR0);           // wait until conversion is complete
    result = ADC14->MEM[5];         // immediately store value in variable
    voltage = result * 0.0002;

    P2->OUT ^= BIT2;

//    printf("ADC value is: \n\t %.0lf\n", result);       // print ADC value
//    printf("Voltage is: \n\t %.2lf\n", voltage);        // print voltage value

    TIMER_A1->CCR[1] = result * 0.0609;                               // set duty cycle

    TIMER32_2->LOAD = 50000;          // Set to a count down of 0.5 second on 3 MHz clock
}
