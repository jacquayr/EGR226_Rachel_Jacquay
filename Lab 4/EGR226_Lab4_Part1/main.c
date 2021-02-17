#include "msp432.h"

void DebounceSwitch1(const int delay);  // debounce function

int main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    P1->SEL1 &= ~BIT1;      // configure P1.1 as I/O
    P1->SEL0 &= ~BIT1;
    P1->DIR &= ~BIT1;       // P1.1 set as input
    P1->REN |= BIT1;        // P1.1 pull down resistor enabled
    P1->OUT |= BIT1;        // pull up/down is selected by P1 -> OUT

    P2->SEL1 &= ~BIT0;      // configure P2.0 as I/O
    P2->SEL0 &= ~BIT0;
    P2->DIR |= BIT0;
    P2->OUT &= ~BIT0;       // P2.0 set as output pin

    P2->SEL1 &= ~BIT1;      // configure P2.1 as I/O
    P2->SEL0 &= ~BIT1;
    P2->DIR |= BIT1;
    P2->OUT &= ~BIT1;       // P2.1 set as output pin

    P2->SEL1 &= ~BIT2;      // configure P2.2 as I/O
    P2->SEL0 &= ~BIT2;
    P2->DIR |= BIT2;
    P2->OUT &= ~BIT2;       // P2.2 set as output pin

    int i = 0;              // index variable

    while (1) {
        if ((P1->IN & BIT1) != BIT1) {          // if switch is pressed
            DebounceSwitch1(500);               // debounce function call

            if ((P1->IN & BIT1) == BIT1) {      // if switch is pressed still
                i++;                            // increment i by 1
            }
        }
                if (i == 0) {                   // if i equals 0 (starting case)
                    P2->OUT &= ~BIT1;           // set all LEDs to OFF
                    P2->OUT &= ~BIT0;
                    P2->OUT &= ~BIT2;
                }

                else if (i == 1) {              // if i equals 1 (red LED case)
                    P2->OUT |= BIT0;            // turn red LED on
                    P2->OUT &= ~BIT1;           // turn other LEDs off
                    P2->OUT &= ~BIT2;
                }

                else if (i == 2) {              // if i equals 2 (green LED case)
                    P2->OUT |= BIT1;            // turn green LED on
                    P2->OUT &= ~BIT0;           // turn other LEDs off
                    P2->OUT &= ~BIT2;
                }

                else if (i == 3) {              // if i equals 3 (blue LED case)
                    P2->OUT |= BIT2;            // turn blue LED on
                    P2->OUT &= ~BIT1;           // turn other LEDs off
                    P2->OUT &= ~BIT0;
                }

                else {                          // anything other than 0 through 3
                    i = 1;                      // go back to red LED case
                }

            }
        }


void DebounceSwitch1(const int delay) {     // debounce function
    int i;                                  // index variable
    for(i = 0; i < delay; i++)              // cycles through the __delay_cycles function for delay amount
        __delay_cycles(1);                  // delays input
}
