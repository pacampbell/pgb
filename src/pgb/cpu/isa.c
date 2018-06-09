#include <assert.h>
#include <errno.h>
#include <stddef.h>

#include <pgb/cpu/isa.h>
#include <pgb/cpu/private/isa.h>
#include <pgb/cpu/private/isa_constants.h>
#include <pgb/cpu/registers.h>
#include <pgb/debug.h>
#include <pgb/utils.h>

static
struct isa_instruction isa_instruction_set[LR35902_OPCODE_TABLE_SIZE] = {
#define SET(MNEMONIC, ASSEMBLY, OPCODE, OPERATION_CLASS, OPERAND_A, MODIFIER_A, OPERAND_B, MODIFIER_B, NUM_BYTES, C0, C1, FLAG_MASK, FLAG_Z, FLAG_N, FLAG_H, FLAG_C, IS_PREFIX) \
	{#MNEMONIC, ASSEMBLY, OPCODE, NUM_BYTES, {C0, C1}, {FLAG_MASK, FLAG_Z, FLAG_N, FLAG_H, FLAG_C}, IS_PREFIX, {OPERAND_A, OPERAND_B, MODIFIER_A, MODIFIER_B}, OPERATION_CLASS},
#include <pgb/cpu/private/isa.def>
#undef SET
};

static
struct isa_instruction prefix_isa_instruction_set[LR35902_PREFIX_CB_OPCODE_TABLE_SIZE] = {
#define SET(MNEMONIC, ASSEMBLY, OPCODE, OPERATION_CLASS, OPERAND_A, MODIFIER_A, OPERAND_B, MODIFIER_B, NUM_BYTES, C0, C1, FLAG_MASK, FLAG_Z, FLAG_N, FLAG_H, FLAG_C, IS_PREFIX) \
	{#MNEMONIC, ASSEMBLY, OPCODE, NUM_BYTES, {C0, C1}, {FLAG_MASK, FLAG_Z, FLAG_N, FLAG_H, FLAG_C}, IS_PREFIX, {OPERAND_A, OPERAND_B, MODIFIER_A, MODIFIER_B}, OPERATION_CLASS},
#include <pgb/cpu/private/prefix_cb.def>
#undef SET
};

int isa_get_instruction(uint8_t opcode, struct isa_instruction **instruction)
{
	*instruction = &isa_instruction_set[opcode];
	return 0;
}

int isa_prefix_cb_get_instruction(uint8_t opcode, struct isa_instruction **instruction)
{
	*instruction = &prefix_isa_instruction_set[opcode];
	return 0;
}
