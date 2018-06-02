#ifndef PGBA_CPU_DECODER_H
#define PGBA_CPU_DECODER_H

#include <stdbool.h>
#include <stdint.h>

struct device;
struct decoded_instruction;

enum decoder_type {
	DECODER_TYPE_LOGICAL,
	DECODER_TYPE_TABLE
};

struct decoder {
	enum decoder_type type;
	int (*decode)(struct device *device, uint8_t opcode, bool is_prefix, struct decoded_instruction *decoded_instruction);
};

int cpu_decoder_configure_decoder(enum decoder_type type, struct decoder *decoder);
int string_to_decoder_type(const char *str, enum decoder_type *type);

#endif /* PGBA_CPU_DECODER_H */
