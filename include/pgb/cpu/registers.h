#ifndef PGB_CPU_REGISTERS_H
#define PGB_CPU_REGISTERS_H

#include <stdint.h>

struct registers {
	union {
		struct {
			union {
				struct {
					uint8_t rsvd        : 4;
					uint8_t carry       : 1;
					uint8_t half_carry  : 1;
					uint8_t subtraction : 1;
					uint8_t zero        : 1;
				} flags;
				uint8_t f;
			};
			uint8_t a;
		};
		uint16_t af;
	};
	union {
		struct {
			uint8_t c;
			uint8_t b;
		};
		uint16_t bc;
	};
	union {
		struct {
			uint8_t e;
			uint8_t d;
		};
		uint16_t de;
	};
	union {
		struct {
			uint8_t l;
			uint8_t h;
		};
		uint16_t hl;
	};
	uint16_t sp;
	uint16_t pc;
} __attribute__((packed));

int registers_init(struct registers *registers);

#endif /* PGB_CPU_REGISTERS_H */
