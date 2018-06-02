#ifndef PGBA_CPU_DECODER_H
#define PGBA_CPU_DECODER_H

#include <stdint.h>

struct decoded_instruction;

enum decoder_type {
	DECODER_TYPE_LOGICAL,
	DECODER_TYPE_TABLE
};

struct decoder {
	enum decoder_type type;
	int (*decode)(uint8_t opcode, uint8_t *instruction_buffer, struct decoded_instruction *decoded_instruction);
};

int cpu_decoder_configure_decoder(enum decoder_type type, struct decoder *decoder);
int string_to_decoder_type(const char *str, enum decoder_type *type);

#endif /* PGBA_CPU_DECODER_H */
