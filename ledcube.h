

#include "MKL25Z4.h"

typedef struct {
	uint8_t b;
	uint8_t g;
	uint8_t r;
} tsColor;

typedef union {
	tsColor rgb;
	uint32_t all;
} tuColor;

typedef struct {
	tuColor color[9];
} tsLayer;

typedef struct {
	tsLayer layer[3];
} tsFrame;



void cube_init ();
void cube_test ();

void cube_spi_write(uint8_t d);
inline void cube_set_z(uint8_t d);
void cube_write_driver(uint32_t d);
void cube_output_enable(uint8_t d);

inline void cube_latch_on();
inline void cube_latch_off();

inline void cube_output_on();
inline void cube_output_off();

void cube_set_single_frame_led(uint32_t frame, uint8_t layer, uint8_t led, uint8_t r, uint8_t g, uint8_t b);

void cube_run();

void cube_dbg();


tsFrame* cube_frame();

