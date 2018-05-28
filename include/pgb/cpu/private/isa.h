#ifndef PGB_CPU_PRIVATE_ISA_H
#define PGB_CPU_PRIVATE_ISA_H

#include <stdint.h>
#include <stdbool.h>
#include <pgb/cpu/registers.h>

#define FLAG_MODIFIER_RESET_AFTER_INSTRUCTION	0
#define FLAG_MODIFIER_FLAG_SET			1
#define FLAG_MODIFIER_AFFECTED_BY_FUNCTION	2
#define FLAG_MODIFIER_NOT_USED			3

struct timing {
	size_t c0;
	size_t c1;
};

struct flag_modifiers {
	uint8_t flag_mask;
	uint8_t flag_z;
	uint8_t flag_n;
	uint8_t flag_h;
	uint8_t flag_c;
};

struct isa_instruction {
	const char *name;
	uint8_t opcode;
	size_t num_bytes;
	struct timing timing;
	struct flag_modifiers flag_modifiers;
	bool is_prefix;
	int (*isa_instruction)(struct registers *registers, uint8_t *data);
};

int isa_get_instruction(uint8_t opcode, struct isa_instruction **instruction);

#endif /* PGB_CPU_PRIVATE_ISA_H */
