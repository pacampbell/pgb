#ifndef PGBA_CPU_DECODER_H
#define PGBA_CPU_DECODER_H

#include <stdbool.h>
#include <stdint.h>

struct device;
struct instruction_info;

enum decoder_type {
	DECODER_TYPE_LOGICAL,
	DECODER_TYPE_TABLE
};

struct decoded_instruction {
	union {
		uint8_t u8;
		int8_t i8;
		uint16_t u16;
	} a;
	union {
		uint8_t u8;
		int8_t i8;
		uint16_t u16;
	} b;
	uint8_t raw_data[4];
	struct instruction_info *info;
};

struct decoder {
	enum decoder_type type;
	int (*decode)(struct device *device, uint8_t opcode, bool is_prefix, struct decoded_instruction *decoded_instruction);
};

int cpu_decoder_configure_decoder(enum decoder_type type, struct decoder *decoder);
int string_to_decoder_type(const char *str, enum decoder_type *type);

int cpu_decoder_get_instruction(uint8_t opcode, struct instruction_info **instruction_info);
int cpu_decoder_get_prefix_cb_instruction(uint8_t opcode, struct instruction_info **instruction_info);

#endif /* PGBA_CPU_DECODER_H */
