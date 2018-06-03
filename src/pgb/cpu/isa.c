#include <assert.h>
#include <errno.h>
#include <stddef.h>

#include <pgb/cpu/isa.h>
#include <pgb/cpu/private/isa.h>
#include <pgb/cpu/registers.h>
#include <pgb/debug.h>
#include <pgb/utils.h>

/*
Instruction STOP has according to manuals opcode 10 00 and thus is 2 bytes long. Anyhow it seems there is no reason for it so some assemblers code it simply as one byte instruction 10.
Flags affected are always shown in Z H N C order. If flag is marked by "0" it means it is reset after the instruction. If it is marked by "1" it is set. If it is marked by "-" it is not changed. If it is marked by "Z", "N", "H" or "C" corresponding flag is affected as expected by its function.

d8  means immediate 8 bit data
d16 means immediate 16 bit data
a8  means 8 bit unsigned data, which are added to $FF00 in certain instructions (replacement for missing IN and OUT instructions)
a16 means 16 bit address
r8  means 8 bit signed data, which are added to program counter

LD A,(C) has alternative mnemonic LD A,($FF00+C)
LD C,(A) has alternative mnemonic LD ($FF00+C),A
LDH A,(a8) has alternative mnemonic LD A,($FF00+a8)
LDH (a8),A has alternative mnemonic LD ($FF00+a8),A
LD A,(HL+) has alternative mnemonic LD A,(HLI) or LDI A,(HL)
LD (HL+),A has alternative mnemonic LD (HLI),A or LDI (HL),A
LD A,(HL-) has alternative mnemonic LD A,(HLD) or LDD A,(HL)
LD (HL-),A has alternative mnemonic LD (HLD),A or LDD (HL),A
LD HL,SP+r8 has alternative mnemonic LDHL SP,r8
*/

static
struct isa_instruction isa_instruction_set[] = {
#define SET(MNEMONIC, ASSEMBLY, OPCODE, OPERATION_CLASS, OPERAND_A, MODIFIER_A, OPERAND_B, MODIFIER_B, NUM_BYTES, C0, C1, FLAG_MASK, FLAG_Z, FLAG_N, FLAG_H, FLAG_C, IS_PREFIX) \
	{#MNEMONIC, ASSEMBLY, OPCODE, NUM_BYTES, {C0, C1}, {FLAG_MASK, FLAG_Z, FLAG_N, FLAG_H, FLAG_C}, IS_PREFIX, {OPERAND_A, OPERAND_B, MODIFIER_A, MODIFIER_B}, OPERATION_CLASS},
#include <pgb/cpu/private/isa.def>
#undef SET
};

static
struct isa_instruction prefix_isa_instruction_set[] = {
#define SET(MNEMONIC, ASSEMBLY, OPCODE, OPERATION_CLASS, OPERAND_A, MODIFIER_A, OPERAND_B, MODIFIER_B, NUM_BYTES, C0, C1, FLAG_MASK, FLAG_Z, FLAG_N, FLAG_H, FLAG_C, IS_PREFIX) \
	{#MNEMONIC, ASSEMBLY, OPCODE, NUM_BYTES, {C0, C1}, {FLAG_MASK, FLAG_Z, FLAG_N, FLAG_H, FLAG_C}, IS_PREFIX, {OPERAND_A, OPERAND_B, MODIFIER_A, MODIFIER_B}, OPERATION_CLASS},
#include <pgb/cpu/private/prefix_cb.def>
#undef SET
};

int isa_get_instruction(uint8_t opcode, struct isa_instruction **instruction)
{
	unsigned i;

	// XXX: Make lookup faster
	for (i = 0; i < ARRAY_SIZE(isa_instruction_set); i++) {
		if (opcode == isa_instruction_set[i].opcode) {
			*instruction = &isa_instruction_set[i];
			return 0;
		}
	}

	return -EINVAL;
}

int isa_prefix_cb_get_instruction(uint8_t opcode, struct isa_instruction **instruction)
{
	unsigned i;

	// XXX: Make lookup faster
	for (i = 0; i < ARRAY_SIZE(prefix_isa_instruction_set); i++) {
		if (opcode == prefix_isa_instruction_set[i].opcode) {
			*instruction = &prefix_isa_instruction_set[i];
			return 0;
		}
	}

	return -EINVAL;
}
