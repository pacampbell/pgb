#include <errno.h>
#include <string.h>

#include <pgb/cpu/cpu.h>
#include <pgb/cpu/decoder.h>
#include <pgb/cpu/instruction_info.h>
#include <pgb/debug.h>
#include <pgb/debugger/debugger.h>
#include <pgb/device/device.h>

enum operand_option {
	OPERAND_OPTION_A,
	OPERAND_OPTION_B
};

#define CHOOSE_VALUE(operand, decoded_instruction, TYPE) ((operand) == OPERAND_OPTION_A ? (decoded_instruction).a.TYPE : (decoded_instruction).b.TYPE)

static
int parse_u16(uint16_t value, char **__str)
{
	int size;
	char *str;

	size = snprintf(NULL, 0, "; 0x%04x", value);
	size += 1;

	str = calloc(size, sizeof(char));
	OK_OR_RETURN(str != NULL, -ENOMEM);

	sprintf(str, "; 0x%04x", value);

	*__str = str;

	return 0;
}

static
int parse_i16(int16_t value, char **__str)
{
	int size;
	char *str;

	size = snprintf(NULL, 0, "; %d", value);
	size += 1;

	str = calloc(size, sizeof(char));
	OK_OR_RETURN(str != NULL, -ENOMEM);

	sprintf(str, "; %d", value);

	*__str = str;

	return 0;
}

static
int literal_to_string(enum operand_option operand, struct decoded_instruction *decoded_instruction, enum instruction_operand_type operand_type, char **comment)
{
	int ret = 0;

	switch (operand_type) {
	case INSTRUCTION_OPERAND_TYPE_I8:
	case INSTRUCTION_OPERAND_TYPE_I16:
		ret = parse_i16(CHOOSE_VALUE(operand, *decoded_instruction, i8), comment);
		break;
	case INSTRUCTION_OPERAND_TYPE_U3:
	case INSTRUCTION_OPERAND_TYPE_U8:
	case INSTRUCTION_OPERAND_TYPE_U16:
		ret = parse_u16(CHOOSE_VALUE(operand, *decoded_instruction, u16), comment);
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

int debugger_fetch_instructions(struct device *device, struct debugger_info *info, size_t n)
{
	int ret;
	size_t i;
	bool is_prefix;
	uint8_t opcode;
	uint16_t pc_saved, address;
	struct decoded_instruction decoded_instruction;
	char *comment_a, *comment_b;

	pc_saved = device->cpu.registers.pc;

	for (i = 0; i < n; i++) {
		memset(&decoded_instruction, 0, sizeof(decoded_instruction));
		comment_a = NULL;
		comment_b = NULL;

		address = device->cpu.registers.pc;

		ret = fetch(device, &opcode, &is_prefix);
		OK_OR_GOTO(ret == 0, reset_pc_and_exit);

		ret = decode(device, opcode, is_prefix, &decoded_instruction);
		OK_OR_GOTO(ret == 0, reset_pc_and_exit);

		info[i].assembly = decoded_instruction.info->assembly;
		info[i].address = address;
		info[i].num_bytes = decoded_instruction.info->num_bytes;
		memcpy(info[i].raw_data, decoded_instruction.raw_data, info[i].num_bytes);


		switch (decoded_instruction.info->operands.a.type) {
		case INSTRUCTION_OPERAND_TYPE_I8:
		case INSTRUCTION_OPERAND_TYPE_I16:
		case INSTRUCTION_OPERAND_TYPE_U3:
		case INSTRUCTION_OPERAND_TYPE_U8:
		case INSTRUCTION_OPERAND_TYPE_U16:
			ret = literal_to_string(OPERAND_OPTION_A, &decoded_instruction, decoded_instruction.info->operands.a.type, &comment_a);
			break;
		default:
			ret = 0;
			break;
		}
		OK_OR_GOTO(ret == 0, reset_pc_and_exit);

		switch (decoded_instruction.info->operands.b.type) {
		case INSTRUCTION_OPERAND_TYPE_I8:
		case INSTRUCTION_OPERAND_TYPE_I16:
		case INSTRUCTION_OPERAND_TYPE_U3:
		case INSTRUCTION_OPERAND_TYPE_U8:
		case INSTRUCTION_OPERAND_TYPE_U16:
			ret = literal_to_string(OPERAND_OPTION_B, &decoded_instruction, decoded_instruction.info->operands.b.type, &comment_b);
			break;
		default:
			ret = 0;
			break;
		}
		OK_OR_GOTO(ret == 0, reset_pc_and_exit);

		info[i].comments.a = comment_a;
		info[i].comments.b = comment_b;
	}

reset_pc_and_exit:
	device->cpu.registers.pc = pc_saved;

	// if (ret != 0) {
	// 	for (i = 0)
	// }
	// TODO: Free memory if error occurred

	return ret;
}

void free_debugger_info(struct debugger_info *info, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++) {
		free(info[i].comments.a);
		free(info[i].comments.b);
	}
}
