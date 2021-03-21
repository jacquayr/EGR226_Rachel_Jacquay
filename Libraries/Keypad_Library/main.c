#include "msp.h"
#include "Keypad.h"

void main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    Keypad_Init();                                  // initialize keypad
    SysTick_Init();                                 // initialize systick

    while(1) {
        pressed = Read_Keypad();                    // call keypad read function and output to pressed
        if (pressed) {                              // if a 1 is returned
            Print_Keys();                        // call print keys function with num variable
            SysTick_Delay(100);                     // delay
            }
        }
    }
