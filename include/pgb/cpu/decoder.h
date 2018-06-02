#ifndef PGBA_CPU_DECODER_H
#define PGBA_CPU_DECODER_H

#include <stdint.h>

struct isa_instruction;
struct decoded_instruction;

int cpu_decoder_decode_instruction(struct isa_instruction *isa_instruction, uint8_t *instruction_buffer,
				   struct decoded_instruction *decoded_instruction);

#endif /* PGBA_CPU_DECODER_H */
