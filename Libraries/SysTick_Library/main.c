#include "msp.h"
#include "SysTick.h"

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	SysTick_Init();

	while (1) {
	    SysTick_Delay(1);
	}
}
