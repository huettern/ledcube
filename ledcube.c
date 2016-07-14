


//==============================================================
// includes
//==============================================================
#include "ledcube.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
#define DRV_READ GPIOD_PDOR

#define Z0 (1 << 12)
#define Z1 (1 << 4)
#define Z2 (1 << 5)

#define LE (1 << 0)
#define OE (1 << 3)


//==============================================================
// static Data
//==============================================================
static volatile uint32_t tickctr = 0;

static volatile uint16_t pwm_ctr = 0;
static const uint16_t pwm_ctr_mod = 255;
static volatile uint8_t z_ctr = 0;
static const uint8_t z_ctr_mod = 3;
static volatile uint32_t frame_ctr = 0;
static const uint32_t frame_ctr_mod = 8; // number of frames

static volatile uint32_t driv_buf = 0; // spi driver buffer holding data for next write
static uint32_t z_buf;

static volatile uint8_t int_flag = 1; // set to calculate new frame

static volatile uint8_t change_z = 0;

static tsFrame frame;


// static tsFrame test_frame[] = {
    // 	{
    // 	0,255,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0, //layer 0
    // 	0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0, //layer 1
    // 	0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0 //layer 2
    // 	},
    // 	{
    // 	0,255,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0, //layer 0
    // 	0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0, //layer 1
    // 	0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0 //layer 2
    // 	},
    // 	{
    // 	0,255,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0, //layer 0
    // 	0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0, //layer 1
    // 	0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0 //layer 2
    // 	},
    // 	{
    // 	0,255,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0, //layer 0
    // 	0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0, //layer 1
    // 	0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0 //layer 2
    // 	},
    // 	{
    // 	0,0,200,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0, //layer 0
    // 	0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0, //layer 1
    // 	0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0 //layer 2
    // 	},
    // 	{
    // 	0,0,200,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0, //layer 0
    // 	0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0, //layer 1
    // 	0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0 //layer 2
    // 	},
    // 	{
    // 	0,0,200,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0, //layer 0
    // 	0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0, //layer 1
    // 	0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0 //layer 2
    // 	},
    // 	{
    // 	0,0,200,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0, //layer 0
    // 	0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0, //layer 1
    // 	0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0,
    // 	 0,0,0,0,0,0,0,0,0 //layer 2
    // 	},
    // };

//==============================================================
// prototypes
//==============================================================
void init_io();
void init_clk();

//==============================================================
// functions
//==============================================================
void cube_init () {
	tsFrame f;
	int i,j,k;
    iprintf("cube_init\r\n");
    RGB_LED(100,0,0);

    init_io();
    cube_set_z(0);

	// SPI
	spi_init();
	cube_latch_on();
	cube_write_driver(0x00000000);
	cube_latch_off();
    RGB_LED(0,100,0);
    init_clk();

    // memset(&test_frame, 0, sizeof(test_frame));
    // // init test frame
    // test_frame.layer[0].color[0].r = 0xff;
    // test_frame.layer[0].color[0].g = 0;
    // test_frame.layer[0].color[0].b = 0;

    // test_frame.layer[1].color[0].r = 0;
    // test_frame.layer[1].color[0].g = 0;
    // test_frame.layer[1].color[0].b = 0;

    // test_frame.layer[2].color[0].r = 0;
    // test_frame.layer[2].color[0].g = 0;
    // test_frame.layer[2].color[0].b = 0xff;

    // test_frame = f;

    // for(i=0;i<3;i++) {
    // 	for(j=0;j<9;j++) {
    // 		iprintf("fram0: layer %d:  ", i, j);
    // 		iprintf("r=%d ", test_frame.layer[i].color[j].r);
    // 		iprintf("g=%d ", test_frame.layer[i].color[j].g);
    // 		iprintf("b=%d\r\n", test_frame.layer[i].color[j].b);
    // 	}
    // }

    // test_frame.layer[0].color[3].rgb.r = 0;
    // test_frame.layer[0].color[3].rgb.g = 0;
    // test_frame.layer[0].color[3].rgb.b = 0xff;
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

void init_clk() {

    SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK;
    SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);

 //    TPM1_MOD  = 2614; // input: 48MHz -> tick at 18.362kHz
	// TPM1_SC   = TPM_SC_CMOD(1) | TPM_SC_PS(0); // start timer

    TPM1_MOD  = 800; // input: 48MHz -> tick at 18.362kHz
	TPM1_SC   = TPM_SC_CMOD(1) | TPM_SC_PS(0); // start timer

 //    TPM1_MOD  = 0xffff; // input: 48MHz -> tick at 18.362kHz
	// TPM1_SC   = TPM_SC_CMOD(1) | TPM_SC_PS(7); // start timer
	
	TPM1_SC |= TPM_SC_TOIE_MASK; //enable overflow interrupt

	enable_irq(INT_TPM1);
}

void cube_run() {
	uint8_t led_ctr = 0;
	uint32_t buf = 0;
	uint8_t led = 0;
	uint8_t z_ctr_old = 0;
	tsColor* clr;
	if(int_flag) {
		// increment counters
		pwm_ctr++;
		z_ctr_old = z_ctr;
		z_ctr+=(uint8_t)(pwm_ctr/pwm_ctr_mod); // inc z ctr after pwm complete
		frame_ctr+=(uint8_t)(z_ctr/z_ctr_mod); // inc frame ctr after z complete

		// wrap counters
		pwm_ctr %= pwm_ctr_mod;
		z_ctr %= z_ctr_mod;
		frame_ctr %= frame_ctr_mod;

		if(z_ctr_old != z_ctr) change_z = 1;

		// for every led
		for(led_ctr = 0; led_ctr < 9; led_ctr++) {
			// get led color
			clr = &frame.layer[z_ctr].color[led_ctr].rgb;
			led = 0;
			// red
			if(pwm_ctr < clr->r) led |= (1<<0);
			// grn
			if(pwm_ctr < clr->g) led |= (1<<1);
			// blu
			if(pwm_ctr < clr->b) led |= (1<<2);

			buf |= (led & 0x07) << (led_ctr*3);
		}
		driv_buf = buf;
		z_buf = (((1<<z_ctr) & 0x01)<<12) | (((1<<z_ctr) & 0x02)<<3) | (((1<<z_ctr) & 0x04)<<3);
		int_flag = 0;

		cube_latch_on(); // write into latch
		cube_write_driver(driv_buf);

		//iprintf("z:%d p:%d zbuf:%d change_z:%d GPIOA_PDOR:%d driv_buf:%d\r\n", z_ctr, pwm_ctr, z_buf,change_z,GPIOA_PDOR,driv_buf);
	}
}

void FTM1_IRQHandler() {
	// TPM1_SC |= TPM_SC_TOF_MASK;
	// return;
	//if(int_flag) RGB_LED(100,0,0);
	if(TPM1_SC & TPM_SC_TOIE_MASK) {
		// tickctr++;
		// if(tickctr < 20) return;
		// tickctr = 0;
		
		// cube_latch_on(); // write into latch
		// cube_write_driver(driv_buf);
		// cube_latch_off(); // write into latch

		// cube_output_off();
		GPIOA_PDOR = (GPIOA_PDOR & ~(Z0 | Z1 | Z2)) | z_buf;
		cube_latch_off();
		// cube_output_on();

		
		// 	cube_output_off();
		// // write spi driver
		// if(change_z) {
		// 	change_z = 0;
		// 	//GPIOA_PDOR = (GPIOA_PDOR & ~(Z0 | Z1 | Z2)) | (((1<<z_ctr) & 0x01)<<12) | (((1<<z_ctr) & 0x02)<<3) | (((1<<z_ctr) & 0x04)<<3);
		// 	cube_output_off();
		// 	GPIOA_PDOR = (GPIOA_PDOR & ~(Z0 | Z1 | Z2)) | z_buf;
		// 	cube_latch_off();
		// 	cube_output_on();
		// }
		// cube_latch_off();
		// cube_output_on();

		// calc new buffer
		int_flag = 1;
		TPM1_SC |= TPM_SC_TOF_MASK;







		// // write spi driver
		// cube_latch_on(); // write into latch
		// cube_write_driver(driv_buf);
		// // controll z driver
		// if(z_ctr_old != z_ctr) {
		// 	d = 1<<z_ctr;
		// 	cube_output_off();
		// 	GPIOA_PDOR = (GPIOA_PDOR & ~(Z0 | Z1 | Z2)) | ((d & 0x01)<<12) | ((d & 0x02)<<3) | ((d & 0x04)<<3);
		// 	cube_latch_off();
		// 	cube_output_on();
		// }
		// // cube_set_z(1<<z_ctr);
		// cube_latch_off();

		// // increment counters
		// pwm_ctr++;
		// z_ctr_old = z_ctr;
		// z_ctr+=(uint8_t)(pwm_ctr/pwm_ctr_mod); // inc z ctr after pwm complete
		// frame_ctr+=(uint8_t)(z_ctr/z_ctr_mod); // inc frame ctr after z complete

		// // wrap counters
		// pwm_ctr %= pwm_ctr_mod;
		// z_ctr %= z_ctr_mod;
		// frame_ctr %= frame_ctr_mod;

		// calc new buffer
		// int_flag = 1;
	}


	// static uint32_t pat = 1;
	// if(TPM1_SC & TPM_SC_TOIE_MASK) {
	// 	TPM1_SC |= TPM_SC_TOF_MASK;
	// 	TPM1_C0SC |= TPM_CnSC_CHF_MASK;
	// 	TPM1_STATUS = 0xffffffff; // clear all tpm1 flags
	// 	cube_write_driver(pat);
	// 	if(pat == 0x80000000) pat = 1; else pat <<= 1;
	// 	tickctr++;
	//     //iprintf("p=%d\r\n",pat);
	// 	return;
	// }
	// if(TPM1_C0SC & TPM_CnSC_CHF_MASK) {
	// }
}

void cube_dbg() {
	iprintf("ctr=%d\r\n", tickctr);
}

void cube_spi_write(uint8_t d) {
	spi_send(d);
	GPIOD_PSOR = LE;
	GPIOD_PSOR = OE;
}


inline void cube_set_z(uint8_t d) {
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
	spi_send((d & 0xff000000) >> 24);
	spi_send((d & 0x00ff0000) >> 16);
	spi_send((d & 0x0000ff00) >> 8);
	spi_send((d & 0x000000ff) >> 0);
}

inline void cube_latch_on() {
	// DRV_CLR = LE; // write into latch
	DRV_SET = LE; // enable latch
}

inline void cube_latch_off() {

	DRV_CLR = LE; // write into latch
	// DRV_SET = LE; // enable latch
}

inline void cube_output_on() {
	DRV_SET = OE;
}
inline void cube_output_off() {
	DRV_CLR = OE;
}


void cube_set_single_frame_led(uint32_t frame, uint8_t layer, uint8_t led, uint8_t r, uint8_t g, uint8_t b) {
	// test_frame[frame].layer[layer].color[led].r=r;
	// test_frame[frame].layer[layer].color[led].g=g;
	// test_frame[frame].layer[layer].color[led].b=b;
}

tsFrame* cube_frame() {
	return &frame;
}