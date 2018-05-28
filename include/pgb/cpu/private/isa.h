#ifndef PGB_CPU_PRIVATE_ISA_H
#define PGB_CPU_PRIVATE_ISA_H

#include <stdint.h>
#include <stdbool.h>
#include <pgb/device/device.h>

#define FLAG_MODIFIER_RESET_AFTER_INSTRUCTION	0
#define FLAG_MODIFIER_FLAG_SET			1
#define FLAG_MODIFIER_AFFECTED_BY_FUNCTION	2
#define FLAG_MODIFIER_NOT_USED			3

struct timing {
	size_t c0;
	size_t c1;
};

struct flag_modifiers {
	uint8_t flag_mask;
	uint8_t flag_z;
	uint8_t flag_n;
	uint8_t flag_h;
	uint8_t flag_c;
};

enum isa_operation {
	/* Core operations */
	ISA_OPERATION_ADC,
	ISA_OPERATION_ADD,
	ISA_OPERATION_ADDC,
	ISA_OPERATION_AND,
	ISA_OPERATION_CALL,
	ISA_OPERATION_CCF,
	ISA_OPERATION_CP,
	ISA_OPERATION_CPL,
	ISA_OPERATION_DAA,
	ISA_OPERATION_DEC,
	ISA_OPERATION_DI,
	ISA_OPERATION_EI,
	ISA_OPERATION_HALT,
	ISA_OPERATION_INC,
	ISA_OPERATION_JP,
	ISA_OPERATION_JR,
	ISA_OPERATION_LD,
	ISA_OPERATION_LDD,
	ISA_OPERATION_LDH,
	ISA_OPERATION_LDHL,
	ISA_OPERATION_LDI,
	ISA_OPERATION_NOP,
	ISA_OPERATION_OR,
	ISA_OPERATION_POP,
	ISA_OPERATION_PREFIX,
	ISA_OPERATION_PUSH,
	ISA_OPERATION_RET,
	ISA_OPERATION_RETI,
	ISA_OPERATION_RST,
	ISA_OPERATION_SBC,
	ISA_OPERATION_SCF,
	ISA_OPERATION_STOP,
	ISA_OPERATION_SUB,
	ISA_OPERATION_XOR,
	/* Prefix Operations */
	ISA_OPERATION_BIT,
	ISA_OPERATION_RES,
	ISA_OPERATION_RL,
	ISA_OPERATION_RLA,
	ISA_OPERATION_RLC,
	ISA_OPERATION_RLCA,
	ISA_OPERATION_RR,
	ISA_OPERATION_RRA,
	ISA_OPERATION_RRC,
	ISA_OPERATION_RRCA,
	ISA_OPERATION_SET,
	ISA_OPERATION_SLA,
	ISA_OPERATION_SRA,
	ISA_OPERATION_SRL,
	ISA_OPERATION_SWAP
};

enum isa_operand {
	ISA_OPERAND_NONE,
	ISA_OPERAND_A,
	ISA_OPERAND_B,
	ISA_OPERAND_C,
	ISA_OPERAND_D,
	ISA_OPERAND_E,
	ISA_OPERAND_H,
	ISA_OPERAND_L,
	ISA_OPERAND_AF,
	ISA_OPERAND_BC,
	ISA_OPERAND_DE,
	ISA_OPERAND_HL,
	ISA_OPERAND_SP,
	ISA_OPERAND_PC,
	/* Generic names for immediate values */
	ISA_OPERAND_N8,   // 8-bit integer constant
	ISA_OPERAND_N16,  // 16-bit integer constant
	ISA_OPERAND_E8,   // 8-bit offset [-128, 127]
	ISA_OPERAND_U3,   // 3-bit unsigned integer constant 0-7
	ISA_OPERAND_VEC,   // Used by reset vectors
	/* Special values for jump */
	ISA_OPERAND_Z,
	ISA_OPERAND_NZ,
	ISA_OPERAND_NC
};

enum isa_operand_modifier {
	ISA_OPERAND_MODIFIER_NONE,
	ISA_OPERAND_MODIFIER_MEM_READ_8,
	ISA_OPERAND_MODIFIER_MEM_WRITE_8,
	ISA_OPERAND_MODIFIER_MEM_READ_16,
	ISA_OPERAND_MODIFIER_MEM_WRITE_16
};

enum isa_register {
	ISA_REGISTER_A = 0x7,
	ISA_REGISTER_B = 0x0,
	ISA_REGISTER_C = 0x1,
	ISA_REGISTER_D = 0x2,
	ISA_REGISTER_E = 0x3,
	ISA_REGISTER_H = 0x4,
	ISA_REGISTER_L = 0x5
};

struct isa_instruction {
	const char *mnemonic;
	const char *assembly;
	uint8_t opcode;
	size_t num_bytes;
	struct timing timing;
	struct flag_modifiers flag_modifiers;
	bool is_prefix;
	struct {
		enum isa_operand a;
		enum isa_operand b;
		enum isa_operand_modifier modifier_a;
		enum isa_operand_modifier modifier_b;
	} operands;
	enum isa_operation isa_operation;
};

int isa_get_instruction(uint8_t opcode, struct isa_instruction **instruction);
int isa_prefix_cb_get_instruction(uint8_t opcode, struct isa_instruction **instruction);
int isa_execute_instruction(struct device *device, struct isa_instruction *instruction, uint8_t *data);

#endif /* PGB_CPU_PRIVATE_ISA_H */
