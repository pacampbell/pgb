#ifndef PGB_CPU_REGISTERS_H
#define PGB_CPU_REGISTERS_H

#include <stdint.h>

struct flags {
	union {
		uint8_t c     : 1;
		uint8_t h     : 1;
		uint8_t n     : 1;
		uint8_t z     : 1;
		uint8_t value : 4;
	};
};

struct registers {
	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
	uint8_t h;
	uint8_t l;

	uint16_t pc;
	uint16_t sp;

	struct flags flags;
};

int registers_init(struct registers *registers);

#endif /* PGB_CPU_REGISTERS_H */
