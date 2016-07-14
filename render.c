

//==============================================================
// includes
//==============================================================
#include "render.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freedom.h"
#include "common.h"
#include "ledcube.h"

void render_init() {
	// init MTIM for 24fps tick
	SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;

    TPM0_MOD = 15625; // input: 48MHz -> tick at 18.362kHz
    // for 24fps: 15625
	TPM0_SC   = TPM_SC_CMOD(1) | TPM_SC_PS(7); // start timer
	TPM0_SC |= TPM_SC_TOIE_MASK; //enable overflow interrupt

	enable_irq(INT_TPM0);
}

void render_rainbow() {
	tsFrame* f;
	f = cube_frame();
	f->layer[0].color[0].all = rgb(255,0,0);
	f->layer[0].color[1].all = rgb(255,127,0);
	f->layer[0].color[2].all = rgb(255,255,0);
	f->layer[0].color[3].all = rgb(127,0,0);
	f->layer[0].color[4].all = rgb(127,127,0);
	f->layer[0].color[5].all = rgb(127,255,0);
	f->layer[0].color[6].all = rgb(0,0,0);
	f->layer[0].color[7].all = rgb(0,127,0);
	f->layer[0].color[8].all = rgb(0,255,0);

	f->layer[1].color[0].all = rgb(255,0,127);
	f->layer[1].color[1].all = rgb(255,127,127);
	f->layer[1].color[2].all = rgb(255,255,127);
	f->layer[1].color[3].all = rgb(127,0,127);
	f->layer[1].color[4].all = rgb(127,127,127);
	f->layer[1].color[5].all = rgb(127,255,127);
	f->layer[1].color[6].all = rgb(0,0,127);
	f->layer[1].color[7].all = rgb(0,127,127);
	f->layer[1].color[8].all = rgb(0,255,127);

	f->layer[2].color[0].all = rgb(255,0,255);
	f->layer[2].color[1].all = rgb(255,127,255);
	f->layer[2].color[2].all = rgb(255,255,255);
	f->layer[2].color[3].all = rgb(127,0,255);
	f->layer[2].color[4].all = rgb(127,127,255);
	f->layer[2].color[5].all = rgb(127,255,255);
	f->layer[2].color[6].all = rgb(0,0,255);
	f->layer[2].color[7].all = rgb(0,127,255);
	f->layer[2].color[8].all = rgb(0,255,255);
}


void FTM0_IRQHandler() {
	uint32_t ctr32;
	uint8_t *p;
	tsFrame* f;
	TPM0_SC |= TPM_SC_TOF_MASK;
	f = cube_frame();
	p=f;
	for(ctr32 = 0; ctr32 < sizeof(tsFrame); ctr32++) 
	{
		// p[ctr32]++;
	}
}
