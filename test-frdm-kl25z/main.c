#include "MKL25Z4.h"

void delay(void) {
  int i;
  for (i = 0; i < 1000000; i++) {
  }
}

void toggle_red(void) {
  GPIOB_PTOR = (1 << 18);
}

void toggle_green(void) {
  GPIOB_PTOR = (1 << 19);
}

void toggle_blue(void) {
  GPIOD_PTOR = (1 << 1);
}

void main(void) {
  /* disable COP */
  SIM_COPC = 0;

  /* enable clocks for PORTB and PORTD */
  SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
  SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;

  /* set B18, B19, D1 to GPIO */
  PORTB_PCR18 = PORT_PCR_MUX(1);
  PORTB_PCR19 = PORT_PCR_MUX(1);
  PORTD_PCR1 = PORT_PCR_MUX(1);

  /* set output B18, B19, D1 high (LED off) */
  GPIOB_PSOR = (1 << 18);
  GPIOB_PSOR = (1 << 19);
  GPIOD_PSOR = (1 << 1);

  /* set B18, B19, D1 DDR to output */
  GPIOB_PDDR |= (1 << 18);
  GPIOB_PDDR |= (1 << 19);
  GPIOD_PDDR |= (1 << 1);

  while (1) {
    toggle_red();
    delay();
    toggle_red();
    toggle_green();
    delay();
    toggle_green();
    toggle_blue();
    delay();
    toggle_blue();
  }
}
