#ifndef PGB_CPU_PRIVATE_LOGICAL_DECODER_H
#define PGB_CPU_PRIVATE_LOGICAL_DECODER_H

struct device;
struct decoded_instruction;

int cpu_logical_decoder_decode(struct device *device, uint8_t opcode, bool is_prefix,
			       struct decoded_instruction *decoded_instruction);

#endif /* PGB_CPU_PRIVATE_LOGICAL_DECODER_H */
