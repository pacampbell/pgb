#ifndef PGB_CPU_PRIVATE_ISA_H
#define PGB_CPU_PRIVATE_ISA_H

#include <stdint.h>
#include <pgb/cpu/registers.h>

struct isa_instruction {
	const char *name;
	int (*isa_instruction)(struct registers *registers, uint8_t *data);
};

// extern struct isa_instruction isa_instructions[256];

#endif /* PGB_CPU_PRIVATE_ISA_H */
