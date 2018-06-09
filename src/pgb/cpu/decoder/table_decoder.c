#include <errno.h>
#include <stdint.h>
#include <string.h>

#include <pgb/cpu/decoder.h>
#include <pgb/cpu/instruction_info.h>
#include <pgb/cpu/private/decoder.h>
#include <pgb/cpu/private/lr35902.h>
#include <pgb/debug.h>
#include <pgb/device/device.h>

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
void dump_instruction(struct device *device, struct instruction_info *instruction_info, uint8_t *instruction_buffer, size_t size)
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

	printf(" %s", instruction_info->assembly);
	if (size > 1) {
		printf("\t; ($%x)", immediate);
	}
	printf("\n");
}

static
int fill_instruction_buffer(struct device *device, uint8_t opcode, struct instruction_info *instruction_info,
			    uint8_t *instruction_buffer, size_t ib_size)
{
	struct cpu *cpu;
	struct registers *registers;
	struct rom_image *rom_image;
	size_t num_bytes;

	cpu = &device->cpu;
	registers = &cpu->registers;
	rom_image = &cpu->rom_image;
	num_bytes = instruction_info->num_bytes;

	OK_OR_RETURN(registers->pc + (num_bytes - 1) < rom_image->size, -EINVAL);
	OK_OR_RETURN(num_bytes <= ib_size, -EINVAL);

	memcpy(instruction_buffer, rom_image->data + registers->pc, num_bytes);
	registers->pc += num_bytes;

	dump_instruction(device, instruction_info, instruction_buffer, num_bytes);

	return 0;
}

int cpu_table_decoder_decode(struct device *device, uint8_t opcode, bool is_prefix, struct decoded_instruction *decoded_instruction)
{
	int ret;
	struct instruction_info *instruction_info;
	uint8_t instruction_buffer[4];

	if (is_prefix) {
		ret = cpu_decoder_get_prefix_cb_instruction(opcode, &instruction_info);
	} else {
		ret = cpu_decoder_get_instruction(opcode, &instruction_info);
	}
	OK_OR_RETURN(ret == 0, ret);
	OK_OR_RETURN(instruction_info->instruction_class != INSTRUCTION_CLASS_INVALID, -EINVAL);

	ret = fill_instruction_buffer(device, opcode, instruction_info, instruction_buffer, sizeof(instruction_buffer));
	OK_OR_RETURN(ret == 0, ret);

	switch (instruction_info->operands.a.type) {
	case INSTRUCTION_OPERAND_TYPE_U8:
	case INSTRUCTION_OPERAND_TYPE_I8:
		ret = extract_u8_from_ib(instruction_buffer, sizeof(instruction_buffer), &decoded_instruction->a.u8);
		break;
	case INSTRUCTION_OPERAND_TYPE_U16:
		ret = extract_u16_from_ib(instruction_buffer, sizeof(instruction_buffer), &decoded_instruction->b.u16);
		break;
	default:
		ret = 0;
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (instruction_info->operands.b.type) {
	case INSTRUCTION_OPERAND_TYPE_U8:
	case INSTRUCTION_OPERAND_TYPE_I8:
		ret = extract_u8_from_ib(instruction_buffer, sizeof(instruction_buffer), &decoded_instruction->b.u8);
		break;
	case INSTRUCTION_OPERAND_TYPE_U16:
		ret = extract_u16_from_ib(instruction_buffer, sizeof(instruction_buffer), &decoded_instruction->b.u16);
		break;
	default:
		ret = 0;
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	decoded_instruction->info = instruction_info;

	return 0;
}
