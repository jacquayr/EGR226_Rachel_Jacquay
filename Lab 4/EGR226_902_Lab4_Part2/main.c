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

    int i = 0;

    while (1) {
        while ((P1->IN & BIT1) != BIT1) {

                if (i == 0) {
                    P2->OUT |= BIT0;
                    P2->OUT &= ~BIT1;
                    P2->OUT &= ~BIT2;
                    DebounceSwitch1(200000);

                    if ((P1->IN & BIT1) != BIT1) {
                        i = 1;
                    }
                }

                else if (i == 1) {
                    P2->OUT |= BIT1;
                    P2->OUT &= ~BIT0;
                    P2->OUT &= ~BIT2;
                    DebounceSwitch1(200000);

                    if ((P1->IN & BIT1) != BIT1) {
                        i = 2;
                    }
                }

                else if (i == 2) {
                    P2->OUT |= BIT2;
                    P2->OUT &= ~BIT1;
                    P2->OUT &= ~BIT0;
                    DebounceSwitch1(200000);

                    if ((P1->IN & BIT1) != BIT1) {
                        i = 3;
                    }
                }

                else if (i == 3) {
                    i = 0;
                }
            }
        }
    }


void DebounceSwitch1(const int delay) {
    int i;
    for(i = 0; i < delay; i++)      //cycles through the __delay_cycles function for delay amount
        __delay_cycles(1);          //delays input
}
