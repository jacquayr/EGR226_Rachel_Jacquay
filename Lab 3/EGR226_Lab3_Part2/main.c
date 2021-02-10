#include "msp.h"

/**
 * main.c
 */

void main(void)
{
    // stop watchdog timer (WDT is used to break you out of unwanted loops, more on this later in the semester)
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	// configure GPIO for output on P1.0 LED1 on MSP432 Launchpad
	P1->DIR = BIT0;

	// temporary variable for loop-maintenance
	int i;
	while (1) {
	    P1->OUT ^= BIT0;     // toggle LED status
	    for (i = 20000; i > 0; i--);
	}
}
