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

    while (1) {                                         // always
        while ((P1->IN & BIT1) != BIT1) {               // while the button is pressed

                if (i == 0) {                           // if i equals 0
                    P2->OUT |= BIT0;                    // turn red LED on
                    P2->OUT &= ~BIT1;                   // turn other LEDs off
                    P2->OUT &= ~BIT2;
                    DebounceSwitch1(200000);            // call debounce function

                    if ((P1->IN & BIT1) != BIT1) {      // if button is still pressed
                        i = 1;                          // increment i
                    }
                }

                else if (i == 1) {                      // if i equals 1
                    P2->OUT |= BIT1;                    // turn green LED on
                    P2->OUT &= ~BIT0;                   // turn other LEDs off
                    P2->OUT &= ~BIT2;
                    DebounceSwitch1(200000);            // call debounce function

                    if ((P1->IN & BIT1) != BIT1) {      // if button is still pressed
                        i = 2;                          // increment i
                    }
                }

                else if (i == 2) {                      // if i equals 2
                    P2->OUT |= BIT2;                    // turn blue LED on
                    P2->OUT &= ~BIT1;                   // turn other LEDs off
                    P2->OUT &= ~BIT0;
                    DebounceSwitch1(200000);            // call debounce function

                    if ((P1->IN & BIT1) != BIT1) {      // if button is still pressed
                        i = 0;                          // set i equal to 0
                    }
                }
            }
        }
    }

/*--------------------------------------------------------------
 * Function:        DebounceSwitch1
 * Description:     Add a delay to make sure the button is
 *                  being pressed and not just bouncing.
 *
 * Inputs:          (const int) delay: integer to set the
 *                  length of the timer delay
 *
 * Outputs:         none
 *-------------------------------------------------------------*/
void DebounceSwitch1(const int delay) {     // debounce function definition
    int i;                                  // index variable
    for(i = 0; i < delay; i++)              //cycles through the __delay_cycles function for delay amount
        __delay_cycles(1);                  //delays input
}
