#ifndef PGB_CPU_CLOCK_H
#define PGB_CPU_CLOCK_H

#include <stdint.h>

struct clock {
	uint16_t m;
	uint16_t t;
};

void clock_init();

#endif /* PGB_CPU_CLOCK_H */
