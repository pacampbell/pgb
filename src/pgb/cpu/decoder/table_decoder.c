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
int isa_register_to_decoded_register(uint8_t isa_register, enum instruction_register *instruction_register)
{
	int ret = 0;

	switch (isa_register) {
	case ISA_REGISTER_A:
		*instruction_register = INSTRUCTION_REGISTER_A;
		break;
	case ISA_REGISTER_B:
		*instruction_register = INSTRUCTION_REGISTER_B;
		break;
	case ISA_REGISTER_C:
		*instruction_register = INSTRUCTION_REGISTER_C;
		break;
	case ISA_REGISTER_D:
		*instruction_register = INSTRUCTION_REGISTER_D;
		break;
	case ISA_REGISTER_E:
		*instruction_register = INSTRUCTION_REGISTER_E;
		break;
	case ISA_REGISTER_F:
		*instruction_register = INSTRUCTION_REGISTER_F;
		break;
	case ISA_REGISTER_H:
		*instruction_register = INSTRUCTION_REGISTER_H;
		break;
	case ISA_REGISTER_L:
		*instruction_register = INSTRUCTION_REGISTER_L;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

static
int cpu_decoder_decode_adc_a_reg(struct isa_instruction *isa_instruction, uint8_t *instruction_buffer,
				 struct decoded_instruction *instruction)
{
	int ret;
	uint8_t opcode, reg;
	enum instruction_register translated_register;

	opcode = isa_instruction->opcode;
	reg = DECODER_OPCODE_GET_FLD(ADC_A_REG, ADDEND, opcode);

	instruction->a.type = INSTRUCTION_OPERAND_TYPE_REGISTER8;
	instruction->b.type = INSTRUCTION_OPERAND_TYPE_REGISTER8;

	instruction->a.reg = INSTRUCTION_REGISTER_A;


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
	enum instruction_register translated_register;

	opcode = isa_instruction->opcode;
	dst = DECODER_OPCODE_GET_FLD(LD_REG_REG, DST, opcode);
	src = DECODER_OPCODE_GET_FLD(LD_REG_REG, SRC, opcode);

	instruction->a.type = INSTRUCTION_OPERAND_TYPE_REGISTER8;
	instruction->b.type = INSTRUCTION_OPERAND_TYPE_REGISTER8;

	ret = isa_register_to_decoded_register(src, &translated_register);
	OK_OR_RETURN(ret == 0, ret);

	instruction->a.reg = translated_register;

	ret = isa_register_to_decoded_register(dst, &translated_register);
	OK_OR_RETURN(ret == 0, ret);

	instruction->b.reg = translated_register;

	return 0;
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
	case LR35902_OPCODE_ADD_A_B: case LR35902_OPCODE_ADD_A_C: case LR35902_OPCODE_ADD_A_D: case LR35902_OPCODE_ADD_A_E:
	case LR35902_OPCODE_ADD_A_H: case LR35902_OPCODE_ADD_A_L: case LR35902_OPCODE_ADD_A_A:
	case LR35902_OPCODE_ADC_A_B: case LR35902_OPCODE_ADC_A_C: case LR35902_OPCODE_ADC_A_D: case LR35902_OPCODE_ADC_A_E:
	case LR35902_OPCODE_ADC_A_H: case LR35902_OPCODE_ADC_A_L: case LR35902_OPCODE_ADC_A_A:
		ret = cpu_decoder_decode_adc_a_reg(isa_instruction, instruction_buffer, decoded_instruction);
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
