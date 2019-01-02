#include <errno.h>
#include <string.h>

#include <pgb/cpu/decoder.h>
#include <pgb/cpu/instruction_info.h>
#include <pgb/cpu/private/lr35902.h>
#include <pgb/cpu/logical_decoder.h>
#include <pgb/cpu/table_decoder.h>
#include <pgb/debug.h>
#include <pgb/utils.h>

int string_to_decoder_type(const char *str, enum decoder_type *type)
{
	int ret = 0;

	if (strcmp(str, "table") == 0) {
		*type  = DECODER_TYPE_TABLE;
	} else if (strcmp(str, "logical") == 0) {
		*type  = DECODER_TYPE_LOGICAL;
	} else {
		ret = -EINVAL;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

int cpu_decoder_configure_decoder(enum decoder_type type, struct decoder *decoder)
{
	int ret = 0;

	switch (type) {
	case DECODER_TYPE_LOGICAL:
		decoder->type = DECODER_TYPE_LOGICAL;
		decoder->decode = cpu_logical_decoder_decode;
		break;
	case DECODER_TYPE_TABLE:
		decoder->type = DECODER_TYPE_TABLE;
		decoder->decode = cpu_table_decoder_decode;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

static
struct instruction_info isa_instruction_set[LR35902_OPCODE_TABLE_SIZE] = {
#define SET(MNEMONIC, ASSEMBLY, OPCODE, OPERATION_CLASS, OPERAND_A, TYPE_A, MODIFIER_A, OPERAND_B, TYPE_B, MODIFIER_B, NUM_BYTES, C0, C1, FLAG_MASK, FLAG_Z, FLAG_N, FLAG_H, FLAG_C, IS_PREFIX) \
	{#MNEMONIC, ASSEMBLY, OPCODE, NUM_BYTES, IS_PREFIX, {C0, C1}, {FLAG_MASK, FLAG_Z, FLAG_N, FLAG_H, FLAG_C}, {{TYPE_A, OPERAND_A, MODIFIER_A}, {TYPE_B, OPERAND_B, MODIFIER_B}}, OPERATION_CLASS},
#include <pgb/cpu/private/lr35902_ops.def>
#undef SET
};

static
struct instruction_info prefix_isa_instruction_set[LR35902_PREFIX_CB_OPCODE_TABLE_SIZE] = {
#define SET(MNEMONIC, ASSEMBLY, OPCODE, OPERATION_CLASS, OPERAND_A, TYPE_A, MODIFIER_A, OPERAND_B, TYPE_B, MODIFIER_B, NUM_BYTES, C0, C1, FLAG_MASK, FLAG_Z, FLAG_N, FLAG_H, FLAG_C, IS_PREFIX) \
	{#MNEMONIC, ASSEMBLY, OPCODE, NUM_BYTES, IS_PREFIX, {C0, C1}, {FLAG_MASK, FLAG_Z, FLAG_N, FLAG_H, FLAG_C}, {{TYPE_A, OPERAND_A, MODIFIER_A}, {TYPE_B, OPERAND_B, MODIFIER_B}}, OPERATION_CLASS},
#include <pgb/cpu/private/lr35902_prefix_cb_ops.def>
#undef SET
};

int cpu_decoder_get_instruction(uint8_t opcode, struct instruction_info **instruction)
{
	OK_OR_RETURN(opcode < ARRAY_SIZE(isa_instruction_set), -EINVAL);

	*instruction = &isa_instruction_set[opcode];

	return 0;
}

int cpu_decoder_get_prefix_cb_instruction(uint8_t opcode, struct instruction_info **instruction)
{
	OK_OR_RETURN(opcode < ARRAY_SIZE(prefix_isa_instruction_set), -EINVAL);

	*instruction = &prefix_isa_instruction_set[opcode];

	return 0;
}

