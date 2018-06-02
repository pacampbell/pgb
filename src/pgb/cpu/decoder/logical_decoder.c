#include <errno.h>
#include <stdint.h>

#include <pgb/cpu/decoder.h>
#include <pgb/debug.h>
#include <pgb/cpu/isa.h>
#include <pgb/cpu/private/decoder.h>
#include <pgb/cpu/private/isa.h>
#include <pgb/cpu/private/isa_appendix.h>

static
int cpu_logical_decoder_prefix_cb_instr(uint8_t opcode, uint8_t *instruction_buffer, struct decoded_instruction *decoded_instruction)
{
	int ret;
	uint8_t x;

	x = BF_GET_FLD(LR35902_OPCODE_FIELD_X, opcode);

	switch (x) {
	case 0:
		/* Roll/Shift register or memory location */
		break;
	case 1:
		/* Test bit */
		break;
	case 2:
		/* Reset bit */
		break;
	case 3:
		/* Set bit */
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

static
int cpu_logical_decoder_decode_core_x_3(uint8_t opcode, uint8_t *instruction_buffer, struct decoded_instruction *decoded_instruction)
{
	int ret;
	uint8_t z;

	z = BF_GET_FLD(LR35902_OPCODE_FIELD_Z, opcode);

	switch (z) {
	case 0:
		/* Conditional return */
		break;
	case 1:
		/* POP and various ops */
		break;
	case 2:
		/* Conditional jump */
		break;
	case 3:
		/* Assorted operations */
		break;
	case 4:
		/* conditional call */
		break;
	case 5:
		/* PUSH and various ops */
		break;
	case 6:
		/* Operate on accumulator and immediate operand */
		break;
	case 7:
		/* Restart */
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

static
int cpu_logical_decoder_decode_core_x_2(uint8_t opcode, uint8_t *instruction_buffer, struct decoded_instruction *decoded_instruction)
{
	/* Operate on accumulator and register/memory loacation */
	return 0;
}

static
int cpu_logical_decoder_decode_core_x_1(uint8_t opcode, uint8_t *instruction_buffer, struct decoded_instruction *decoded_instruction)
{
	int ret = 0;
	uint8_t y, z, p, q;

	(void) p; (void) q;

	y = BF_GET_FLD(LR35902_OPCODE_FIELD_Y, opcode);
	z = BF_GET_FLD(LR35902_OPCODE_FIELD_Z, opcode);
	p = BF_GET_FLD(LR35902_OPCODE_FIELD_P, opcode);
	q = BF_GET_FLD(LR35902_OPCODE_FIELD_Q, opcode);

	if (z == 6 && y == 6) {
		/* HALT */
	} else {
		switch (z) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			/* 8-bit loading */
			break;
		default:
			ret = -EINVAL;
			break;
		}
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

static
int cpu_logical_decoder_decode_core_x_0(uint8_t opcode, uint8_t *instruction_buffer, struct decoded_instruction *decoded_instruction)
{
	int ret = 0;
	uint8_t y, z, p, q;

	(void) p; (void) q; (void) y;

	y = BF_GET_FLD(LR35902_OPCODE_FIELD_Y, opcode);
	z = BF_GET_FLD(LR35902_OPCODE_FIELD_Z, opcode);
	p = BF_GET_FLD(LR35902_OPCODE_FIELD_P, opcode);
	q = BF_GET_FLD(LR35902_OPCODE_FIELD_Q, opcode);

	switch (z) {
	case 0:
		/* Relative jumps and assorted ops */
		break;
	case 1:
		/* 16-bit load immediate/add */
		break;
	case 2:
		/* Indirect loading */
		break;
	case 3:
		/* 16-bit INC/DEC */
		break;
	case 4:
		/* 8-bit INC */
		break;
	case 5:
		/* 8 bit dec */
		break;
	case 6:
		/* 8 bit load immediate */
		break;
	case 7:
		/* Assorted operations on accumulator/flags */
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

static
int cpu_logical_decoder_decode_core_instr(uint8_t opcode, uint8_t *instruction_buffer, struct decoded_instruction *decoded_instruction)
{
	int ret;
	uint8_t x;

	x = BF_GET_FLD(LR35902_OPCODE_FIELD_X, opcode);

	switch (x) {
	case 0:
		ret = cpu_logical_decoder_decode_core_x_0(opcode, instruction_buffer, decoded_instruction);
		break;
	case 1:
		ret = cpu_logical_decoder_decode_core_x_1(opcode, instruction_buffer, decoded_instruction);
		break;
	case 2:
		ret = cpu_logical_decoder_decode_core_x_2(opcode, instruction_buffer, decoded_instruction);
		break;
	case 3:
		ret = cpu_logical_decoder_decode_core_x_3(opcode, instruction_buffer, decoded_instruction);
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return 0;
}

int cpu_logical_decoder_decode(uint8_t opcode, uint8_t *instruction_buffer, struct decoded_instruction *decoded_instruction)
{
	int ret;

	switch (opcode) {
	case LR35902_OPCODE_PREFIX_CB:
		ret = cpu_logical_decoder_prefix_cb_instr(opcode, instruction_buffer, decoded_instruction);
		break;
	default:
		ret = cpu_logical_decoder_decode_core_instr(opcode, instruction_buffer, decoded_instruction);
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}