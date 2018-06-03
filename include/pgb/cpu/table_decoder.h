#ifndef PGB_CPU_PRIVATE_TABLE_DECODER_H
#define PGB_CPU_PRIVATE_TABLE_DECODER_H

struct device;
struct decoded_instruction;

int cpu_table_decoder_decode(struct device *device, uint8_t opcode, bool is_prefix,
			     struct decoded_instruction *decoded_instruction);

#endif /* PGB_CPU_PRIVATE_TABLE_DECODER_H */
