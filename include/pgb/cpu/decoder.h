#ifndef PGBA_CPU_DECODER_H
#define PGBA_CPU_DECODER_H

#include <stdbool.h>
#include <stdint.h>

struct device;

enum decoder_type {
	DECODER_TYPE_LOGICAL,
	DECODER_TYPE_TABLE
};

enum decoded_instruction_operand_type {
	DECODED_INSTRUCTION_OPERAND_TYPE_NONE,
	DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8,
	DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER16,
	DECODED_INSTRUCTION_OPERAND_TYPE_INT8,
	DECODED_INSTRUCTION_OPERAND_TYPE_INT16,
	DECODED_INSTRUCTION_OPERAND_TYPE_UINT3,
	DECODED_INSTRUCTION_OPERAND_TYPE_UINT8,
	DECODED_INSTRUCTION_OPERAND_TYPE_UINT16
};

enum decoded_instruction_register {
	DECODED_INSTRUCTION_REGISTER_A,
	DECODED_INSTRUCTION_REGISTER_B,
	DECODED_INSTRUCTION_REGISTER_C,
	DECODED_INSTRUCTION_REGISTER_D,
	DECODED_INSTRUCTION_REGISTER_E,
	DECODED_INSTRUCTION_REGISTER_F,
	DECODED_INSTRUCTION_REGISTER_H,
	DECODED_INSTRUCTION_REGISTER_L,
	DECODED_INSTRUCTION_REGISTER_AF,
	DECODED_INSTRUCTION_REGISTER_BC,
	DECODED_INSTRUCTION_REGISTER_DE,
	DECODED_INSTRUCTION_REGISTER_HL,
	DECODED_INSTRUCTION_REGISTER_SP,
	DECODED_INSTRUCTION_REGISTER_PC
};

struct decoded_instruction {
	struct {
		enum decoded_instruction_operand_type type;
		uint16_t value;
		enum decoded_instruction_register reg;

	} a;
	struct {
		enum decoded_instruction_operand_type type;
		uint16_t value;
		enum decoded_instruction_register reg;
	} b;
	struct isa_instruction *info;
};

struct decoder {
	enum decoder_type type;
	int (*decode)(struct device *device, uint8_t opcode, bool is_prefix, struct decoded_instruction *decoded_instruction);
};

int cpu_decoder_configure_decoder(enum decoder_type type, struct decoder *decoder);
int string_to_decoder_type(const char *str, enum decoder_type *type);

#endif /* PGBA_CPU_DECODER_H */
