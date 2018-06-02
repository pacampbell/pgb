#ifndef PGB_CPU_PRIVATE_DECODER_H
#define PGB_CPU_PRIVATE_DECODER_H

#include <pgb/bit_field.h>

#define DECODER_OPCODE_GET_FLD(OPCODE, FIELD, value) BF_GET_FLD(LR35902_OPCODE_##OPCODE##_FIELD_##FIELD, value)

#endif /* PGB_CPU_PRIVATE_DECODER_H */