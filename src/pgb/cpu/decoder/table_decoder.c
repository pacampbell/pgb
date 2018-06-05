#include <errno.h>
#include <stdint.h>
#include <string.h>

#include <pgb/cpu/decoder.h>
#include <pgb/cpu/isa.h>
#include <pgb/cpu/private/decoder.h>
#include <pgb/cpu/private/isa.h>
#include <pgb/cpu/private/isa_appendix.h>
#include <pgb/debug.h>
#include <pgb/device/device.h>

static
int isa_register_to_decoded_register(uint8_t isa_register, enum decoded_instruction_register *instruction_register)
{
	int ret = 0;

	switch (isa_register) {
	case ISA_REGISTER_A:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_A;
		break;
	case ISA_REGISTER_B:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_B;
		break;
	case ISA_REGISTER_C:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_C;
		break;
	case ISA_REGISTER_D:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_D;
		break;
	case ISA_REGISTER_E:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_E;
		break;
	case ISA_REGISTER_F:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_F;
		break;
	case ISA_REGISTER_H:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_H;
		break;
	case ISA_REGISTER_L:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_L;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

static
int isa_operand_to_decoded_register(enum isa_operand isa_operand, enum decoded_instruction_register *instruction_register)
{
	int ret = 0;

	switch (isa_operand) {
	case ISA_OPERAND_A:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_A;
		break;
	case ISA_OPERAND_B:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_B;
		break;
	case ISA_OPERAND_C:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_C;
		break;
	case ISA_OPERAND_D:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_D;
		break;
	case ISA_OPERAND_E:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_E;
		break;
	case ISA_OPERAND_F:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_F;
		break;
	case ISA_OPERAND_H:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_H;
		break;
	case ISA_OPERAND_L:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_L;
		break;
	case ISA_OPERAND_AF:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_AF;
		break;
	case ISA_OPERAND_BC:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_BC;
		break;
	case ISA_OPERAND_DE:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_DE;
		break;
	case ISA_OPERAND_HL:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_HL;
		break;
	case ISA_OPERAND_SP:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_SP;
		break;
	case ISA_OPERAND_PC:
		*instruction_register = DECODED_INSTRUCTION_REGISTER_SP;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

static
int isa_operand_to_decoded_operand_type(enum isa_operand isa_operand, enum decoded_instruction_operand_type *operand_type)
{
	int ret = 0;

	switch (isa_operand) {
	case ISA_OPERAND_A:
	case ISA_OPERAND_B:
	case ISA_OPERAND_C:
	case ISA_OPERAND_D:
	case ISA_OPERAND_E:
	case ISA_OPERAND_F:
	case ISA_OPERAND_H:
	case ISA_OPERAND_L:
		*operand_type = DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8;
		break;
	case ISA_OPERAND_AF:
	case ISA_OPERAND_BC:
	case ISA_OPERAND_DE:
	case ISA_OPERAND_HL:
	case ISA_OPERAND_SP:
	case ISA_OPERAND_PC:
		*operand_type = DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER16;
		break;
	case ISA_OPERAND_U3:
		*operand_type = DECODED_INSTRUCTION_OPERAND_TYPE_UINT3;
		break;
	case ISA_OPERAND_U8:
		*operand_type = DECODED_INSTRUCTION_OPERAND_TYPE_UINT8;
		break;
	case ISA_OPERAND_U16:
		*operand_type = DECODED_INSTRUCTION_OPERAND_TYPE_UINT16;
		break;
	case ISA_OPERAND_I8:
		*operand_type = DECODED_INSTRUCTION_OPERAND_TYPE_INT8;
		break;
	case ISA_OPERAND_NONE:
		*operand_type = DECODED_INSTRUCTION_OPERAND_TYPE_NONE;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

static
int extract_u16_from_ib(uint8_t *instruction_buffer, size_t size, uint16_t *immediate)
{
	unsigned i;
	uint16_t value;

	value = 0;
	for (i = 1; i < size; i++) {
		value |= instruction_buffer[i] << ((i - 1) * 8);
	}

	*immediate = value;

	return 0;
}

static
int extract_u8_from_ib(uint8_t *instruction_buffer, size_t size, uint8_t *immediate)
{
	unsigned i;
	uint8_t value;

	value = 0;
	for (i = 1; i < size; i++) {
		value |= instruction_buffer[i] << ((i - 1) * 8);
	}

	*immediate = value;

	return 0;
}

static
int cpu_decoder_decode_adc_a_reg(struct isa_instruction *isa_instruction, uint8_t *instruction_buffer,
				 struct decoded_instruction *instruction)
{
	int ret;
	uint8_t opcode, reg;
	enum decoded_instruction_register translated_register;

	opcode = isa_instruction->opcode;
	reg = DECODER_OPCODE_GET_FLD(ADC_A_REG, ADDEND, opcode);

	instruction->a.type = DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8;
	instruction->b.type = DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8;

	instruction->a.reg = DECODED_INSTRUCTION_REGISTER_A;


	ret = isa_register_to_decoded_register(reg, &translated_register);
	OK_OR_RETURN(ret == 0, ret);

	instruction->b.reg = translated_register;

	return 0;
}

static
int cpu_decoder_decode_ld_reg_reg(struct isa_instruction *isa_instruction, uint8_t *instruction_buffer,
				  struct decoded_instruction *instruction)
{
	int ret;
	uint8_t opcode, dst, src;
	enum decoded_instruction_register translated_register;

	opcode = isa_instruction->opcode;
	dst = DECODER_OPCODE_GET_FLD(LD_REG_REG, DST, opcode);
	src = DECODER_OPCODE_GET_FLD(LD_REG_REG, SRC, opcode);

	instruction->a.type = DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8;
	instruction->b.type = DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8;

	ret = isa_register_to_decoded_register(src, &translated_register);
	OK_OR_RETURN(ret == 0, ret);

	instruction->b.reg = translated_register;

	ret = isa_register_to_decoded_register(dst, &translated_register);
	OK_OR_RETURN(ret == 0, ret);

	instruction->a.reg = translated_register;

	return 0;
}

static
int cpu_decoder_decode_ld_reg16_u16(struct isa_instruction *isa_instruction, uint8_t *instruction_buffer,
				    struct decoded_instruction *instruction)
{
	int ret;
	uint16_t immediate;
	size_t size;
	enum decoded_instruction_register translated_register;

	size = isa_instruction->num_bytes;
	instruction->a.type = DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER16;
	instruction->b.type = DECODED_INSTRUCTION_OPERAND_TYPE_UINT16;

	ret = isa_operand_to_decoded_register(isa_instruction->operands.a, &translated_register);
	OK_OR_RETURN(ret == 0, ret);

	ret = extract_u16_from_ib(instruction_buffer, size, &immediate);
	OK_OR_RETURN(ret == 0, ret);

	instruction->a.reg = translated_register;
	instruction->b.value = immediate;

	return 0;
}

static
int cpu_decoder_decode_ld_reg8_u8(struct isa_instruction *isa_instruction, uint8_t *instruction_buffer,
				  struct decoded_instruction *instruction)
{
	int ret;
	uint8_t immediate;
	size_t size;
	enum decoded_instruction_register translated_register;

	size = isa_instruction->num_bytes;
	instruction->a.type = DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8;
	instruction->b.type = DECODED_INSTRUCTION_OPERAND_TYPE_UINT8;

	ret = isa_operand_to_decoded_register(isa_instruction->operands.a, &translated_register);
	OK_OR_RETURN(ret == 0, ret);

	ret = extract_u8_from_ib(instruction_buffer, size, &immediate);
	OK_OR_RETURN(ret == 0, ret);

	instruction->a.reg = translated_register;
	instruction->b.value = immediate;

	return 0;
}

static
int cpu_decoder_decode_inc_reg(struct isa_instruction *isa_instruction, uint8_t *instruction_buffer,
			       struct decoded_instruction *instruction)
{
	int ret;

	ret = isa_operand_to_decoded_operand_type(isa_instruction->operands.a, &instruction->a.type);
	OK_OR_RETURN(ret == 0, ret);

	ret = isa_operand_to_decoded_register(isa_instruction->operands.a, &instruction->a.reg);
	OK_OR_WARN(ret == 0);

	return ret;
}

static
int cpu_decoder_decode_dec_reg(struct isa_instruction *isa_instruction, uint8_t *instruction_buffer,
			       struct decoded_instruction *instruction)
{
	int ret;

	ret = isa_operand_to_decoded_operand_type(isa_instruction->operands.a, &instruction->a.type);
	OK_OR_RETURN(ret == 0, ret);

	ret = isa_operand_to_decoded_register(isa_instruction->operands.a, &instruction->a.reg);
	OK_OR_WARN(ret == 0);

	return ret;
}

static
int cpu_decoder_decode_prefix_instruction(struct isa_instruction *isa_instruction, uint8_t *instruction_buffer,
					  struct decoded_instruction *instruction)
{
	return 0;
}

static
int cpu_decoder_decode_core_instruction(struct isa_instruction *isa_instruction, uint8_t *instruction_buffer,
					struct decoded_instruction *decoded_instruction)
{
	int ret = 0;

	switch (isa_instruction->opcode) {
	case LR35902_OPCODE_INVALID_DB: case LR35902_OPCODE_INVALID_DD: case LR35902_OPCODE_INVALID_E3:
	case LR35902_OPCODE_INVALID_E4: case LR35902_OPCODE_INVALID_EB: case LR35902_OPCODE_INVALID_EC:
	case LR35902_OPCODE_INVALID_ED: case LR35902_OPCODE_INVALID_F4: case LR35902_OPCODE_INVALID_FC:
	case LR35902_OPCODE_INVALID_FD:
		ret = -EINVAL;
		break;
	case LR35902_OPCODE_LD_B_B: case LR35902_OPCODE_LD_B_C: case LR35902_OPCODE_LD_B_D: case LR35902_OPCODE_LD_B_E:
	case LR35902_OPCODE_LD_B_H: case LR35902_OPCODE_LD_B_L: case LR35902_OPCODE_LD_B_A:
	case LR35902_OPCODE_LD_C_B: case LR35902_OPCODE_LD_C_C: case LR35902_OPCODE_LD_C_D: case LR35902_OPCODE_LD_C_E:
	case LR35902_OPCODE_LD_C_H: case LR35902_OPCODE_LD_C_L: case LR35902_OPCODE_LD_C_A:
	case LR35902_OPCODE_LD_D_B: case LR35902_OPCODE_LD_D_C: case LR35902_OPCODE_LD_D_D: case LR35902_OPCODE_LD_D_E:
	case LR35902_OPCODE_LD_D_H: case LR35902_OPCODE_LD_D_L: case LR35902_OPCODE_LD_D_A:
	case LR35902_OPCODE_LD_E_B: case LR35902_OPCODE_LD_E_C: case LR35902_OPCODE_LD_E_D: case LR35902_OPCODE_LD_E_E:
	case LR35902_OPCODE_LD_E_H: case LR35902_OPCODE_LD_E_L: case LR35902_OPCODE_LD_E_A:
	case LR35902_OPCODE_LD_H_B: case LR35902_OPCODE_LD_H_C: case LR35902_OPCODE_LD_H_D: case LR35902_OPCODE_LD_H_E:
	case LR35902_OPCODE_LD_H_H: case LR35902_OPCODE_LD_H_L: case LR35902_OPCODE_LD_H_A:
	case LR35902_OPCODE_LD_L_B: case LR35902_OPCODE_LD_L_C: case LR35902_OPCODE_LD_L_D: case LR35902_OPCODE_LD_L_E:
	case LR35902_OPCODE_LD_L_H: case LR35902_OPCODE_LD_L_L: case LR35902_OPCODE_LD_L_A:
	case LR35902_OPCODE_LD_A_B: case LR35902_OPCODE_LD_A_C: case LR35902_OPCODE_LD_A_D: case LR35902_OPCODE_LD_A_E:
	case LR35902_OPCODE_LD_A_H: case LR35902_OPCODE_LD_A_L: case LR35902_OPCODE_LD_A_A:
		ret = cpu_decoder_decode_ld_reg_reg(isa_instruction, instruction_buffer, decoded_instruction);
		break;
	case LR35902_OPCODE_LD_BC_U16: case LR35902_OPCODE_LD_DE_U16: case LR35902_OPCODE_LD_HL_U16:
	case LR35902_OPCODE_LD_SP_U16:
		ret = cpu_decoder_decode_ld_reg16_u16(isa_instruction, instruction_buffer, decoded_instruction);
		break;
	case LR35902_OPCODE_LD_B_U8: case LR35902_OPCODE_LD_C_U8: case LR35902_OPCODE_LD_D_U8:
	case LR35902_OPCODE_LD_E_U8: case LR35902_OPCODE_LD_H_U8: case LR35902_OPCODE_LD_L_U8:
	case LR35902_OPCODE_LD_A_U8:
		ret = cpu_decoder_decode_ld_reg8_u8(isa_instruction, instruction_buffer, decoded_instruction);
		break;
	case LR35902_OPCODE_LD_SP_HL:
		break;
	case LR35902_OPCODE_ADD_A_B: case LR35902_OPCODE_ADD_A_C: case LR35902_OPCODE_ADD_A_D: case LR35902_OPCODE_ADD_A_E:
	case LR35902_OPCODE_ADD_A_H: case LR35902_OPCODE_ADD_A_L: case LR35902_OPCODE_ADD_A_A:
	case LR35902_OPCODE_ADC_A_B: case LR35902_OPCODE_ADC_A_C: case LR35902_OPCODE_ADC_A_D: case LR35902_OPCODE_ADC_A_E:
	case LR35902_OPCODE_ADC_A_H: case LR35902_OPCODE_ADC_A_L: case LR35902_OPCODE_ADC_A_A:
		ret = cpu_decoder_decode_adc_a_reg(isa_instruction, instruction_buffer, decoded_instruction);
		break;
	case LR35902_OPCODE_INC_B: case LR35902_OPCODE_INC_C: case LR35902_OPCODE_INC_D: case LR35902_OPCODE_INC_E:
	case LR35902_OPCODE_INC_H: case LR35902_OPCODE_INC_L: case LR35902_OPCODE_INC_A:
	case LR35902_OPCODE_INC_BC: case LR35902_OPCODE_INC_DE: case LR35902_OPCODE_INC_HL: case LR35902_OPCODE_INC_SP:
		ret = cpu_decoder_decode_inc_reg(isa_instruction, instruction_buffer, decoded_instruction);
		break;
	case LR35902_OPCODE_DEC_B: case LR35902_OPCODE_DEC_C: case LR35902_OPCODE_DEC_D: case LR35902_OPCODE_DEC_E:
	case LR35902_OPCODE_DEC_H: case LR35902_OPCODE_DEC_L: case LR35902_OPCODE_DEC_A:
	case LR35902_OPCODE_DEC_BC: case LR35902_OPCODE_DEC_DE: case LR35902_OPCODE_DEC_HL: case LR35902_OPCODE_DEC_SP:
		ret = cpu_decoder_decode_dec_reg(isa_instruction, instruction_buffer, decoded_instruction);
		break;
	default:
		/* TODO: If we get here the instruction is not implemented */
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

static
void dump_instruction(struct device *device, struct isa_instruction *isa_instruction, uint8_t *instruction_buffer, size_t size)
{
	unsigned i, j;
	uint16_t immediate = 0;

	if (!IS_DEBUG())
		return;


	for (i = 1; i < size; i++) {
		immediate |= instruction_buffer[i] << ((i - 1) * 8);
	}

	printf("%04x ", device->cpu.registers.pc - (uint16_t)size);

	for (j = 0; j < 3 - size; j++) {
		printf("  ");
	}

	for (j = 0; j < size; j++) {
		printf("%02x", instruction_buffer[j]);
	}

	printf(" %s", isa_instruction->assembly);
	if (size > 1) {
		printf("\t; ($%x)", immediate);
	}
	printf("\n");
}

static
int fill_instruction_buffer(struct device *device, uint8_t opcode, struct isa_instruction *isa_instruction,
			    uint8_t *instruction_buffer, size_t ib_size)
{
	struct cpu *cpu;
	struct registers *registers;
	struct rom_image *rom_image;
	size_t num_bytes;

	cpu = &device->cpu;
	registers = &cpu->registers;
	rom_image = &cpu->rom_image;
	num_bytes = isa_instruction->num_bytes;

	OK_OR_RETURN(registers->pc + (num_bytes - 1) < rom_image->size, -EINVAL);
	OK_OR_RETURN(num_bytes <= ib_size, -EINVAL);

	memcpy(instruction_buffer, rom_image->data + registers->pc, num_bytes);
	registers->pc += num_bytes;

	dump_instruction(device, isa_instruction, instruction_buffer, num_bytes);

	return 0;
}

int cpu_table_decoder_decode(struct device *device, uint8_t opcode, bool is_prefix, struct decoded_instruction *decoded_instruction)
{
	int ret;
	struct isa_instruction *isa_instruction;
	uint8_t instruction_buffer[4];

	if (is_prefix) {
		ret = isa_prefix_cb_get_instruction(opcode, &isa_instruction);
	} else {
		ret = isa_get_instruction(opcode, &isa_instruction);
	}
	OK_OR_RETURN(ret == 0, ret);

	ret = fill_instruction_buffer(device, opcode, isa_instruction, instruction_buffer, sizeof(instruction_buffer));
	OK_OR_RETURN(ret == 0, ret);

	if (is_prefix) {
		ret = cpu_decoder_decode_prefix_instruction(isa_instruction, instruction_buffer, decoded_instruction);
	} else {
		ret = cpu_decoder_decode_core_instruction(isa_instruction, instruction_buffer, decoded_instruction);
	}
	OK_OR_RETURN(ret == 0, ret);

	decoded_instruction->info = isa_instruction;

	return 0;
}
