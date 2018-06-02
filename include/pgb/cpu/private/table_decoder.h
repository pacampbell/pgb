#ifndef PGB_CPU_PRIVATE_TABLE_DECODER_H
#define PGB_CPU_PRIVATE_TABLE_DECODER_H

int cpu_table_decoder_decode(uint8_t opcode, uint8_t *instruction_buffer, struct decoded_instruction *decoded_instruction);

#endif /* PGB_CPU_PRIVATE_TABLE_DECODER_H */
