


//==============================================================
// includes
//==============================================================
#include "ledcube.h"

#include <stdio.h>
#include "freedom.h"
#include "common.h"
#include "spi.h"

//==============================================================
// Defines
//==============================================================
#define Z_SET GPIOA_PSOR
#define Z_CLR GPIOA_PCOR
#define Z_TOG GPIOA_PTOR
#define Z_READ GPIOA_PDOR


#define DRV_SET GPIOD_PSOR
#define DRV_CLR GPIOD_PCOR
#define DRV_TOG GPIOD_PTOR

#define Z0 (1 << 12)
#define Z1 (1 << 4)
#define Z2 (1 << 5)

#define LE (1 << 0)
#define OE (1 << 3)

//==============================================================
// prototypes
//==============================================================
void init_io();


//==============================================================
// functions
//==============================================================
void cube_init () {
    iprintf("cube_init\r\n");
    RGB_LED(100,0,0);

    init_io();

	// SPI
	spi_init();

    RGB_LED(0,100,0);

}

void cube_test () {
	Z_SET = Z2;
	Z_CLR = Z0;
	Z_CLR = Z1;

	cube_spi_write(0x02);
	GPIOD_PSOR = LE;
	GPIOD_PSOR = OE;
	for(;;) {


	// Z_SET = Z2;
	// Z_CLR = Z0;
	// Z_CLR = Z1;
		cube_set_z(0x01);
        delay(200);
	// Z_SET = Z1;
	// Z_CLR = Z2;
	// Z_CLR = Z0;
		cube_set_z(0x02);
        delay(200);
	// Z_SET = Z0;
	// Z_CLR = Z1;
	// Z_CLR = Z2;
		cube_set_z(0x04);
        delay(200);
	}



	// spi_send(0x01);
	// GPIOD_PSOR = LE;
	// GPIOD_PSOR = OE;
}




void init_io() {
	SIM_SCGC5    |= SIM_SCGC5_PORTA_MASK;      /* Enable Clock to Port A */ 
	/* set A4,5,12 to GPIO */
	PORTA_PCR4 = PORT_PCR_MUX(1);
	PORTA_PCR5 = PORT_PCR_MUX(1);
	PORTA_PCR12 = PORT_PCR_MUX(1);

	// set low
	GPIOA_PSOR = Z0;
	GPIOA_PSOR = Z1;
	GPIOA_PSOR = Z2;

	// set output
	GPIOA_PDDR |= Z0;
	GPIOA_PDDR |= Z1;
	GPIOA_PDDR |= Z2;


	// LE and OE
	SIM_SCGC5    |= SIM_SCGC5_PORTD_MASK;      /* Enable Clock to Port D */ 

	PORTD_PCR0 = PORT_PCR_MUX(1);
	PORTD_PCR3 = PORT_PCR_MUX(1);

	DRV_CLR = LE;
	DRV_CLR = OE;

	GPIOD_PDDR |= LE;
	GPIOD_PDDR |= OE;

}

void cube_spi_write(uint8_t d) {
	spi_send(d);
	GPIOD_PSOR = LE;
	GPIOD_PSOR = OE;
}


void cube_set_z(uint8_t d) {
	GPIOA_PDOR = (GPIOA_PDOR & ~(Z0 | Z1 | Z2)) | ((d & 0x01)<<12) | ((d & 0x02)<<3) | ((d & 0x04)<<3);
}

void cube_output_enable(uint8_t d) {
	if(d) {
		DRV_SET = OE;
	} else {
		DRV_CLR = OE;
	}
}

void cube_write_driver(uint32_t d) {
	DRV_CLR = LE; // write into latch

	spi_send((d & 0xff000000) >> 24);
	spi_send((d & 0x00ff0000) >> 16);
	spi_send((d & 0x0000ff00) >> 8);
	spi_send((d & 0x000000ff) >> 0);

	DRV_SET = LE; // enable latch
}