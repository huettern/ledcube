

#include "MKL25Z4.h"

void cube_init ();
void cube_test ();

void cube_spi_write(uint8_t d);
void cube_set_z(uint8_t d);
void cube_write_driver(uint32_t d);
void cube_output_enable(uint8_t d);