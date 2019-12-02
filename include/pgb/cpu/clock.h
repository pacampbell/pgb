#ifndef PGB_CPU_CLOCK_H
#define PGB_CPU_CLOCK_H

#include <stdint.h>

struct clock {
	uint16_t m;
	uint16_t t;
};

int clock_init(struct clock *clock);

#endif /* PGB_CPU_CLOCK_H */
