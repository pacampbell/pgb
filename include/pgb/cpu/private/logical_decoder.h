#ifndef PGB_CPU_PRIVATE_LOGICAL_DECODER_H
#define PGB_CPU_PRIVATE_LOGICAL_DECODER_H

struct decoded_instruction;

int cpu_logical_decoder_decode(uint8_t opcode, uint8_t *instruction_buffer, struct decoded_instruction *decoded_instruction);

#endif /* PGB_CPU_PRIVATE_LOGICAL_DECODER_H */
