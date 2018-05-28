#ifndef PGB_CPU_PRIVATE_PREFIX_CB_H
#define PGB_CPU_PRIVATE_PREFIX_CB_H

#include <stdint.h>
#include <pgb/cpu/private/isa.h>
#include <pgb/cpu/registers.h>

int isa_prefix_cb_get_instruction(uint8_t opcode, struct isa_instruction **instruction);

#endif /* PGB_CPU_PRIVATE_ISA_H */
