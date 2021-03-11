#include "msp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void SysTick_Init(void);
void LCD_Init(void);
void Pin_Init(void);
void SysTick_Delay(uint8_t delay);
void delay_micro(uint32_t microsecond);
void delay_milli(uint32_t millisecond);
void PulseEnablePin(void);
void pushNibble(uint8_t nibble);
void pushByte(uint8_t byte);
void commandWrite(uint32_t command);
void dataWrite(uint32_t data);

void firstname(void);
void lastname(void);
void egr(void);
void num(void);

void line2(void);
void line3(void);
void line4(void);

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    SysTick_Init();                                 // initializations
    Pin_Init();
    LCD_Init();

    commandWrite(0x01);
    delay_milli(500);

    commandWrite(0x85);
    //delay_milli(50);

    firstname();
    //delay_milli(50);

    line2();
    lastname();
    //delay_milli(50);

    line3();
    egr();
    //delay_milli(50);

    line4();
    num();
    //delay_milli(50);

    while (1) {

    }
}

void SysTick_Init(void) {
    SysTick->CTRL = 0;
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->VAL = 0;
    SysTick->CTRL = 0x00000005;
}

void Pin_Init(void) {
    // pin 3.0 = RS
    P3->SEL0 &= ~BIT0;
    P3->SEL1 &= ~BIT0;
    P3->DIR |= BIT0;
    P3->OUT &= ~BIT0;

    // pin 5.7 = E
    P5->SEL0 &= ~BIT7;
    P5->SEL1 &= ~BIT7;
    P5->DIR |= BIT7;
    P5->OUT &= ~BIT7;

    // pin 1.6 = DB4
    P1->SEL0 &= ~BIT6;
    P1->SEL1 &= ~BIT6;
    P1->DIR |= BIT6;
    P1->OUT &= ~BIT6;

    // pin 1.7 = DB5
    P1->SEL0 &= ~BIT7;
    P1->SEL1 &= ~BIT7;
    P1->DIR |= BIT7;
    P1->OUT &= ~BIT7;

    // pin 5.0 = DB6
    P5->SEL0 &= ~BIT0;
    P5->SEL1 &= ~BIT0;
    P5->DIR |= BIT0;
    P5->OUT &= ~BIT0;

    // pin 5.2 = DB7
    P5->SEL0 &= ~BIT2;
    P5->SEL1 &= ~BIT2;
    P5->DIR |= BIT2;
    P5->OUT &= ~BIT2;

    delay_milli(60);
}

void SysTick_Delay(uint8_t delay) {
    SysTick->LOAD = ((delay * 3000) - 1);           // delay for 1 millisecond per delay value
    SysTick->VAL = 0;                               // any write to current value clears it
    while ((SysTick->CTRL & 0x00010000) == 0);      // wait for flag to be set
}

void delay_micro(uint32_t microsecond) {
    SysTick->LOAD = ((microsecond * 3) - 1);
    SysTick->VAL = 0;
    while((SysTick->CTRL & 0x00010000) == 0);
}

void delay_milli(uint32_t millisecond) {
    SysTick->LOAD = ((millisecond * 3000) - 1);
    SysTick->VAL = 0;
    while((SysTick->CTRL & 0x00010000) == 0);
}

void PulseEnablePin(void) {
    P5OUT &= ~BIT7;
    delay_micro(50);

    P5OUT |= BIT7;
    delay_micro(50);

    P5OUT &= ~BIT7;
    delay_micro(50);
}

void pushNibble(uint8_t nibble) {
    P1OUT &= ~0x40;
    P1OUT &= ~0x80;
    P5OUT &= ~0x01;
    P5OUT &= ~0x04;

    // DB4
    if (nibble & BIT0) {
        P1->OUT |= 0x40;
    }

    // DB5
    delay_micro(50);
    if (nibble & BIT1) {
        P1->OUT |= 0x80;
    }

    // DB6
    delay_micro(50);
    if (nibble & BIT2) {
        P5->OUT |= 0x01;
    }

    // DB7
    delay_micro(50);
    if (nibble & BIT3) {
        P5->OUT |= 0x04;
    }

    delay_micro(50);
    PulseEnablePin();
}

void pushByte(uint8_t byte) {
    uint8_t nibble;

    nibble = (byte & 0xF0) >> 4;
    pushNibble(nibble);
    nibble = byte & 0x0F;
    pushNibble(nibble);
    delay_micro(100);
}

void LCD_Init(void) {
    commandWrite(0x3);
    delay_milli(100);
    commandWrite(0x3);
    delay_milli(200);
    commandWrite(0x3);
    delay_milli(100);
    commandWrite(0x2);

    delay_milli(100);
    commandWrite(0x28);
    delay_milli(100);

    delay_milli(100);
    commandWrite(0x0F);
    delay_milli(100);
    commandWrite(0x01);
    delay_milli(100);
    commandWrite(0x06);
    delay_milli(10);
}

void commandWrite(uint32_t command) {
    P3->OUT &= ~BIT0;
    pushByte(command);
}

void dataWrite(uint32_t data) {
    P3->OUT |= BIT0;
    pushByte(data);
}

void firstname(void) {
    int i;
    char firstname[6];
    strcpy(firstname, "Rachel");

    for (i = 0; i < 6; i++) {
        dataWrite(firstname[i]);
        //delay_milli(500);
    }
}

void lastname(void) {
    int i;
    char lastname[7];
    strcpy(lastname, "Jacquay");

    for (i = 0; i < 7; i++) {
        dataWrite(lastname[i]);
        //delay_milli(500);
    }
}

void egr(void) {
    int i;
    char egr[3];
    strcpy(egr, "EGR");

    for (i = 0; i < 3; i++) {
        dataWrite(egr[i]);
        //delay_milli(500);
    }
}

void num(void) {
    int i;
    char num[3];
    strcpy(num, "226");

    for (i = 0; i < 3; i++) {
        dataWrite(num[i]);
        //delay_milli(50);
    }
}

void line2(void) {
    commandWrite(0xC4);
    delay_micro(100);
}

void line3(void) {
    commandWrite(0x96);
    delay_micro(100);
}

void line4(void) {
    commandWrite(0xD6);
    delay_micro(100);
}
