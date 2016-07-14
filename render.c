

//==============================================================
// includes
//==============================================================
#include "render.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freedom.h"
#include "common.h"
#include "spi.h"

void render_init() {
	// init MTIM for 24fps tick
	SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;

	LPTMR0_PSR = LPTMR_PSR_PCS(0) | LPTMR_PSR_PRESCALE(6); //presclae 128
	LPTMR0_CMR = LPTMR_CMR_COMPARE(2); // for 24fps interrupt 15625
	LPTMR0_CSR |= LPTMR_CSR_TIE_MASK | LPTMR_CSR_TCF_MASK; // enable interrupt and timer
	LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;
	enable_irq(INT_LPTimer-16);
}


void LPTimer_IRQHandler() {
	LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;
	iprintf("t");
	RGB_LED(100,100,0);
}
