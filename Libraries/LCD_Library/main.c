#include "msp.h"
#include "LCD.h"


void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    SysTick_Init();                                 // initializations
    Pin_Init();
    LCD_Init();

    while (1) {

    }
}
